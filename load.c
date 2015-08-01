/* Author: ETA Team *
 * Last Modification: 08/01/2015 by Foo*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <math.h>


/* Define stats do jogador */
#define BASE_HP 20
#define BASE_ATTACK 7
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10

/* Define quantidade de itens e tamanho da bag */
#define TAM_BAG 5
#define QUANT_ITENS 6

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {

    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/* Read 1 character without echo */
char getch(void) {
    return getch_(0);
}

/* Read 1 character with echo */
char getche(void) {
    return getch_(1);
}

/* Struct de itens */
typedef struct {
    /* Tipos:
     * 1 - arma
     * 2 - armadura
     * 3 - pot */
    int indice, valor, tipo;
    char nome[51];
} Item;

 /* Define o struct do player */
typedef struct {

	int hp, level, attack, defense, XP, x, y, NextLevel, MaxHP, nivelAtual, con, dext, str, pontos;
	Item weapon, gear;

} Player;

/* Define o struct de inimigos */
typedef struct {

	int hp, attack, defense, givenXP, x, y, dropItems[QUANT_ITENS];
	double dropRate;
	char nome[51];

} Enemy;

typedef struct {

	int wall, player, used, enemyIndice, itemIndice,stairs;

} Map;

typedef struct{

	int nivel, inimigos, tamI, tamJ, indice;
	Map mapa[30][30]; 
} Nivel;

/* Struct da bag */
typedef struct {

	Item item;
	int quantidade, used;
} Bag;


/* Inclui as bibliotecas que: *
 * inicializa o jogo, *
 * realiza as funcoes de combate *
 * executa comandos */
#include "init.h"
#include "combat.h"
#include "comand.h"


/* Imprime o campo e os stats do jogador */
void print(Nivel nivel, Player controller){

	int i, j, porcentHP, porcentXP;

	printf("\n\n");

	/* Imprime o mapa e os stats do jogador a cada movimento */
	for(i = 0; (i < nivel.tamI); i++){
		for(j = 0; (j < nivel.tamJ); j++){

			if((abs(i - controller.y) <= 2) && (abs(j - controller.x) <= 2)){

				if(nivel.mapa[i][j].player)
					printf("P ");

				else if(nivel.mapa[i][j].wall)
					printf("X ");

				else if(nivel.mapa[i][j].enemyIndice >= 0)
					printf("E ");

				else if(nivel.mapa[i][j].itemIndice >= 0)
					printf("I ");

				else if(nivel.mapa[i][j].stairs < 0)
					printf("< ");

				else if(nivel.mapa[i][j].stairs > 0)
					printf("> ");

				else
					printf("  ");
			}
			
			else
				printf("- ");	
		}

		printf("\n");
	}	
	printf("\n\n");

	porcentXP = ceil(((double) controller.XP / controller.NextLevel) * 100);
	porcentHP = ceil(((double) controller.hp / controller.MaxHP) * 100);

	printf("HP: %d/%d (%d%%)\nLevel: %d\nXP: %d/%d (%d%%)\n", controller.hp, controller.MaxHP, porcentHP, controller.level, controller.XP, controller.NextLevel, porcentXP);
	printf("Attack: %d\nDefense: %d\n\n\n", controller.attack, controller.defense);
}


int main (){

	int count = 0, i;
	char comand;
	Nivel *niveis, aux;
	Player player;
	Enemy **enemies;
	Bag *bag;
	Item *itens;
	FILE *database, *arq;

	system("clear");
	
	/* Aloca o vetor que armazena os inimigos e a bag */
	bag = malloc(TAM_BAG * sizeof(Bag));
	itens = malloc(QUANT_ITENS * sizeof(Item));

	database = fopen("database.bin", "rb");

	/* Pega os itens na database de itens */
	if(database != NULL){
		fread(itens, sizeof(Item), QUANT_ITENS, database);
		fclose(database);
	}

	else{
		printf("Erro ao ler database!!\n");
		return 0;
	}

	/* Pega os niveis na database de niveis */
	arq = fopen("database2.bin", "rb");

	if(arq == NULL){
		printf("Erro ao abrir database 2!!\n");
		return 0; 
	}

	while(fread(&aux, sizeof (Nivel), 1, arq))
		count++;

	if(count == 0){
		printf("Nenhum nivel na database!!\n");
		return 0;
	}

	/* Aloca a matriz que guarda todos os inimigos de cada nivel e o vetor de niveis */
	niveis = malloc(count * sizeof(Nivel));
	enemies = malloc(count * sizeof(Enemy*));

	rewind(arq);
	fread(niveis, count * sizeof(Nivel), 1, arq);
	fclose(arq);

	arq = fopen("database3.bin", "rb");

	if(arq == NULL){
		printf("Erro ao abrir database de inimigos!\n");
		return 0;
	}

	for(i = 0; i < count; i++){
		enemies[i] = malloc(niveis[i].inimigos * sizeof(Enemy));
		fread(enemies[i], sizeof(**enemies), niveis[i].inimigos, arq);
	}

	fclose(arq);

	/* Carrega ou inicializa um novo jogo */
	if(gameLoad(&player, niveis, enemies, bag, count) == 0){

		/* Inicializa o player e a mochila */
		playerInit(&player, itens, &niveis[0]);
		bagInit(bag);
		comandList();
	}

	for(i = 0; i < count; i++)
		enemyPositions(niveis[i], enemies[i]);

	/* Loop que executa o jogo */
	do{
		print(niveis[player.nivelAtual], player);
		comand = getch();
		system("clear");
	}while(executeComand(comand, &player, niveis, enemies, bag, itens, count));

	for(i = 0; i < count; i++)
		free(enemies[i]);

	free(niveis);
	free(enemies);
	free(bag);
	free(itens);

	return 0;
}