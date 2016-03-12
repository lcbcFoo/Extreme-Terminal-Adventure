/* Author: ETA Team *
 * Last Modification: 03/11/2016 by Foo*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <curses.h>

/* Inclui as bibliotecas que: *
 * inicializa o jogo, *
 * realiza as funcoes de combate *
 * executa comandos */
#include "init.h"
#include "combat.h"
#include "command.h"
#include "autogen.h"


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
	Nivel nivel, aux;
	Player player;
	Enemy *enemies;
	Bag *bag;
	Item *itens, read;
	FILE *database, *arq;

	initscr();
	start_color(); //Esta função torna possível o uso das cores

//Abaixo estamos definindo os pares de cores que serão utilizados no programa
    init_pair(1,COLOR_WHITE,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_GREEN,COLOR_BLACK);
    init_pair(4,COLOR_BLUE,COLOR_BLACK);
    bkgd(COLOR_PAIR(1)); 

	refresh();

	/* Aloca o vetor que armazena a bag */
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

	enemies = NULL;

	/* Carrega ou inicializa um novo jogo */
	if(gameLoad(&player, &nivel, enemies, bag) == 0){

		/* Inicializa o player e a mochila */
		playerInit(&player, itens);
		nivel = genNivel(0, &player);
		bagInit(bag);
		comandList();
		mvprintw(11,0,"Voce pode rever os comandos disponiveis acessando o MENU.\n\n\nDigite alguma letra para comecar: ");
		comand = getch();
		clear();
	}

	bag[0].item = itens[0];
	bag[0].quantidade = 3;
	bag[0].used = 1;


/*
	enemyPositions(nivel, enemies);
*/
	/* Loop que executa o jogo */
	do{
		print(nivel, player, enemies);
		comand = getch();
		clear();
	}while(executeComand(comand, &player, &nivel, enemies, bag, itens));


	free(enemies);
	free(bag);
	free(itens);

	endwin();

	return 0;
}