/* Author: ETA Team *
 * Last Modification: 02/28/2015 by Foo*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Inclui as bibliotecas que: *
 * inicializa o jogo, *
 * realiza as funcoes de combate *
 * executa comandos */
#include "init.h"
#include "combat.h"
#include "command.h"


/* Define stats do jogador */
#define BASE_HP 20
#define BASE_ATTACK 7
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10

/* Define quantidade de itens e tamanho da bag */
#define TAM_BAG 5


int main (){

	int count = 0, i, existentItems = 0;
	char comand;
	Nivel *niveis, aux;
	Player player;
	Enemy **enemies;
	Bag *bag;
	Item *itens, read;
	FILE *database, *arq;

	system("clear");
	
	/* Aloca o vetor que armazena os inimigos e a bag */
	bag = malloc(TAM_BAG * sizeof(Bag));

	database = fopen("items.bin", "rb");

	while(fread(&read, sizeof(Item), 1, database))
		existentItems++;

	itens = malloc(existentItems * sizeof(Item));

	rewind(database);
	
	/* Pega os itens na database de itens */
	if(database != NULL){
		fread(itens, sizeof(Item), existentItems, database);
		fclose(database);
	}

	else{
		printf("Erro ao ler database!!\n");
		return 0;
	}

	/* Pega os niveis na database de niveis */
	arq = fopen("maps.bin", "rb");

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

	arq = fopen("enemiesOnGame.bin", "rb");

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
		print(niveis[player.nivelAtual], player, enemies);
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