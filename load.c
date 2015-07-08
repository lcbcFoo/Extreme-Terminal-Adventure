/* Author: ETA Team *
 * Last Modification: 07/08/2015 by Chams */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
    return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
    return getch_(1);
}


 /* Define o struct do player */
typedef struct {

	int hp, level, attack, defense, XP, x, y, NextLevel, MaxHP;

} Player;

/* Define o struct de inimigos */
typedef struct {

	int hp, attack, defense, givenXP, x, y;

} Enemy;

/* Define o struct do mapa */
typedef struct {

	int wall, player, used, enemyIndice;

} Map;

typedef struct {
    /* Tipos:
     * 1 - arma
     * 2 - armadura
     * 3 - pot */
    int indice, valor, tipo;
    char nome[51];
} Item;

/* Inclui as bibliotecas que: *
 * inicializa o jogo, *
 * realiza as funcoes de combate *
 * executa comandos */
#include "init.h"
#include "combat.h"
#include "comand.h"

/* Define stats do jogador */
#define BASE_HP 20
#define BASE_ATTACK 5
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10

/* Define stats de cada inimigo */
#define ENEMY_1_HP 10
#define ENEMY_1_BASE_ATTACK 6
#define ENEMY_1_BASE_DEF 3
#define ENEMY_1_XP 11

/* Define tamanho do mapa e quantidade de inimigos */
#define TAM 10
#define NUM_INIMIGOS 4


/* To do list: * 
 *
 * - ELABORAR UMA FORMA DE ADICIONAR CLASSES AOS INIMIGOS, DEPOIS AO PLAYER * 
 *
 * - PENSAR SOBRE NOVOS MOVIMENTOS/ATAQUES (POR EXEMPLO, RANGED) */


/* Imprime o campo e os stats do jogador */
void print(Map map[TAM][TAM], Player controller){

	int i, j;

	printf("\n\n");

	/* Imprime o mapa e os stats do jogador a cada movimento */
	for(i = 0; (i < TAM); i++){
		for(j = 0; (j < TAM); j++){

			if((abs(i - controller.y) <= 2) && (abs(j - controller.x) <= 2)){

				if(map[i][j].player)
					printf("P ");

				else if(map[i][j].wall)
					printf("X ");

				else if(map[i][j].enemyIndice >= 0)
					printf("E ");

				else
					printf("  ");
			}
			
			else
				printf("- ");	
		}

		printf("\n");
	}	
	printf("\n\n");

	printf("HP: %d/%d\nLevel: %d\nXP: %d/%d\n\n\n", controller.hp, controller.MaxHP, controller.level, controller.XP, controller.NextLevel);
}


int main (){

	char comand;
	Player player;
	Map map[TAM][TAM];
	Enemy *enemies;

	system("clear");
	
	/* Aloca o vetor que armazena os inimigos */
	enemies = malloc(NUM_INIMIGOS * sizeof(Enemy));

	/* Carrega ou inicializa um novo jogo */
	if(gameLoad(&player, map, enemies) == 0){
		playerInit(&player);
		mapInit(map, player.y, player.x, enemies);
		comandList();
	}

	/* Loop que executa o jogo */
	do{
		print(map, player);
		comand=getch();
		system("clear");
	}while(executeComand(comand, &player, map, enemies));

	free(enemies);

	return 0;
}