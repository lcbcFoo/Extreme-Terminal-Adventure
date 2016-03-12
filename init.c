/* Author: ETA Team *
 * Last Modification: 03/05/2015 by Foo*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <math.h>
#include <curses.h>
#include "init.h"
#include "command.h"
#include "combat.h"


/* Inicializa o jogador */
void playerInit(Player *player, Item *itens){

	/* Inicializa os stas do jogador com os stats base */
	(*player).hp = BASE_HP;
	(*player).level = 1;
	(*player).XP = 0;
	(*player).NextLevel = BASE_NEXT_LEVEL;
	(*player).MaxHP = BASE_HP;
	(*player).weapon = itens[2];
	(*player).attack = BASE_ATTACK + (*player).weapon.valor;
	(*player).gear = itens[3];
	(*player).defense = BASE_DEF + (*player).gear.valor;
	(*player).con = 0;
	(*player).dext = 0;
	(*player).str = 0;
	(*player).pontos = 0;
}

void bagInit(Bag *bag){

	int i;

	for(i = 0; i < TAM_BAG; i++)
		bag[i].used = 0;

}

void enemyPositions(Nivel nivel, Enemy *enemies){

	int i, j;

	for(i = 0; i < nivel.tamI; i++)
		for(j = 0; j < nivel.tamJ; j++)
			if(nivel.mapa[i][j].enemyIndice >= 0){
				enemies[nivel.mapa[i][j].enemyIndice].y = i;
				enemies[nivel.mapa[i][j].enemyIndice].x = j;
			}
			
}

void print(Nivel nivel, Player controller, Enemy *enemies){

	int i, j, radius, porcentHP, porcentXP;

	porcentXP = ceil(((double) controller.XP / controller.NextLevel) * 100);
	porcentHP = ceil(((double) controller.hp / controller.MaxHP) * 100);

	printw("\nLevel:              %6d\n", controller.level);
	printw("HP:          %6d/%6d  (", controller.hp, controller.MaxHP);
	for(int i = 1; i < 21; i++)
		if(porcentHP >= 5 * i)
			printw("-");
		else
			printw(" ");

	printw(") %d\%\n", porcentHP);
	printw("Experiencia: %6d/%6d  (", controller.XP, controller.NextLevel);

	for(int i = 1; i < 21; i++)
		if(porcentXP >= 5 * i)
			printw("-");
		else
			printw(" ");

	printw(") %d\%\n", porcentXP);

	printw("Attack:             %6d\nDefense:            %6d\n\n\n", controller.attack, controller.defense);

	for(i = 0; (i < nivel.tamI); i++)
		for(j = 0; (j < nivel.tamJ); j++)
			nivel.mapa[i][j].shown = 0;

	nivel.mapa[controller.y][controller.x].shown = 1;

	/* Verifica os espacos que o player consegue ver */
	for(radius = 1; radius < 7; radius++){

		for(i = controller.y - radius; (i < controller.y + radius); i++){
			for(j = controller.x - radius; (j < controller.x + radius); j++){

				if(((i >= 0) && (i < nivel.tamI)) && ((j >= 0) && (j < nivel.tamJ))){

					if(nivel.mapa[i][j].shown == radius){

						if(i - 1 >= 0){ 
							if(nivel.mapa[i - 1][j].wall == 0)
	                       		nivel.mapa[i - 1][j].shown = radius + 1; 

		                    else
		                    	nivel.mapa[i - 1][j].shown = 1;
		                }

	                    if(i + 1 < nivel.tamI){
	                    	if(nivel.mapa[i + 1][j].wall == 0)
	                        	nivel.mapa[i + 1][j].shown = radius + 1;

		                    else
		                    	nivel.mapa[i + 1][j].shown = 1;
	                	}

	                    if(j - 1 >= 0){
	                    	if(nivel.mapa[i][j - 1].wall == 0)
	                        	nivel.mapa[i][j - 1].shown = radius + 1;

		                    else
		                    	nivel.mapa[i][j - 1].shown = 1;
		                }

	                    if(j + 1 < nivel.tamJ){
	                    	if(nivel.mapa[i][j + 1].wall == 0)
	                        	nivel.mapa[i][j + 1].shown = radius + 1;

		                    else
		                    	nivel.mapa[i][j + 1].shown = 1;
	                	}
	                } 

	                if(nivel.mapa[i][j].enemyIndice >= 0)
	                	enemies[nivel.mapa[i][j].enemyIndice].seen = 1;
				}
			}
		}		
	}

	/* Imprime o mapa e os stats do jogador a cada movimento */
	for(i = 0; (i < nivel.tamI); i++){
		for(j = 0; (j < nivel.tamJ * 2); j += 2){

			if(nivel.mapa[i][j/2].shown){

				if(nivel.mapa[i][j/2].player){
					attron(COLOR_PAIR(4));
					printw("P ");
					attron(COLOR_PAIR(1));
				}

				else if(nivel.mapa[i][j/2].wall)
					printw("X ");

				else if(nivel.mapa[i][j/2].enemyIndice >= 0){
					attron(COLOR_PAIR(2));
					printw("E ");
					attron(COLOR_PAIR(1));
				}

				else if(nivel.mapa[i][j/2].itemIndice >= 0){
					attron(COLOR_PAIR(4));
					printw("I ");
					attron(COLOR_PAIR(1));
				}	

				else if(nivel.mapa[i][j/2].stairs < 0){
					attron(COLOR_PAIR(3));
					printw("< ");
					attron(COLOR_PAIR(1));
				}

				else if(nivel.mapa[i][j/2].stairs > 0){
					attron(COLOR_PAIR(3));
					printw("> ");
					attron(COLOR_PAIR(1));
				}

				else
					printw("  ");
			}
			
			else
				printw(". ");	
		}
		printw("\n");
	}
}

/* Verfica se existe e carrega partida salva */
int gameLoad(Player *player, Nivel *nivel, Enemy *enemies, Bag *bag){

	int i;
	char read;
	FILE *arq;

	arq = fopen("savedGame.bin", "rb");

	/* Caso o arquivo de salvar o jogo exista, permite que o jogo seja carregado *
	 * caso contrario retorna 0 e cria um novo mapa */
	if(arq != NULL){
		attron(COLOR_PAIR(3));
		printf("Voce deseja continuar o jogo salvo? (y/n)\n");
		scanf(" %c", &read);

		if(read == 'y'){
			fread(nivel, sizeof(Nivel), 1, arq);
			fread(player, sizeof(Player), 1, arq);

			fread(enemies, nivel->inimigos * sizeof(Enemy), 1, arq);
			
			fread(bag, sizeof(Bag), TAM_BAG, arq);

			fclose(arq);
			remove("savedGame.bin");
			system("clear");
			attron(COLOR_PAIR(1));
			return 1;
		}
		attron(COLOR_PAIR(1));
		clear();

		fclose(arq);	
	}

	return 0;
}