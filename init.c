/* Author: ETA Team *
 * Last Modification: 03/05/2015 by Foo*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <math.h>
#include "init.h"
#include "command.h"
#include "combat.h"

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

	printf("\n\n");

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
		for(j = 0; (j < nivel.tamJ); j++){

			if(nivel.mapa[i][j].shown){

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
				printf(". ");	
		}

		printf("\n");
	}	
	printf("\n\n");

	porcentXP = ceil(((double) controller.XP / controller.NextLevel) * 100);
	porcentHP = ceil(((double) controller.hp / controller.MaxHP) * 100);

	printf("HP: %d/%d (%d%%)\nLevel: %d\nXP: %d/%d (%d%%)\n", controller.hp, controller.MaxHP, porcentHP, controller.level, controller.XP, controller.NextLevel, porcentXP);
	printf("Attack: %d\nDefense: %d\n\n\n", controller.attack, controller.defense);
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
			return 1;
		}

		system("clear");

		fclose(arq);	
	}

	return 0;
}