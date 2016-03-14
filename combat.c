/* Author: ETA Team *
 * Last Modification: 03/11/2016 by Foo*/

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

/* Funcao que recebe XP e verifica se o personagem upou */
void verificaXP(Player *player, int xpGanho){

	int flag = 1;

	(*player).XP += xpGanho;

	while(flag){
		flag = 0;

		/* Verifica se o jogador upou e atualiza os stats do jogador */
		if((*player).XP >= (*player).NextLevel){
			printw("Parabens! Voce atingiu o level %d\n", (*player).level + 1);
			(*player).XP -= (*player).NextLevel;
			(*player).level++;
			(*player).attack += 1.2 * (*player).level * BASE_ATTACK / 10;
			(*player).defense += ceil((*player).level * BASE_DEF / 10);
			(*player).MaxHP += (*player).level * BASE_HP / 10; 
			(*player).hp += (*player).level * BASE_HP / 5;
			(*player).NextLevel *= 1.5;
			(*player).pontos += 5;

			if((*player).hp > (*player).MaxHP)
				(*player).hp = (*player).MaxHP;

			flag = 1;
		}
	}	
}

/* Verifica se algum item sera dropado */
void dropCheck(Enemy *enemy, Map *position){

	int aux;

	srand(time(NULL));

	if((*enemy).dropRate > 0){
		aux = rand();

		aux = aux % 101;

		if(aux <= (*enemy).dropRate * 100){
			do{
				aux = rand() % QUANT_ITENS;
				(*position).used = 1;
				(*position).itemIndice = aux;
				(*position).quantItems = 1;
			}while((*enemy).dropItems[aux] == 0);
		}		
	}
}

/* Funcao que executa o ataque de inimigos */
int enemyAttack(Player *player, Enemy *enemy){

	int damageDone, attack, defense, critChance;

	/* Verifica o dano recebido */
	srand(time(NULL));

	if(rand() % 101 >= 30 * (*player).dext / 100){

		if(rand() % 2)
			attack = ceil((*enemy).attack + (0.1 * (*enemy).attack * (rand() % 100) / (double) 100)); 

		else 
			attack = ceil((*enemy).attack - (0.1 * (*enemy).attack * (rand() % 100) /  (double) 100)); 

		critChance = 10;

		srand(time(NULL));

		if(critChance >= rand() % 101){
			printw("Dano critico! ");
			attack += attack * 0.5;
		}

		defense = ceil((85 * (*player).defense + ((rand() % 16) * (*player).defense)) /  (double) 100);

		damageDone = attack - defense;

		if(damageDone <= 0)
			damageDone = 1;

		(*player).hp -= damageDone;

		/* Indica o danop recebido e verifica se vc morreu*/
		printw("%s causou a voce %d de dano!\n", (*enemy).nome, damageDone);

		if((*player).hp <= 0){
			attron(COLOR_PAIR(2));
			printw("Voce foi morto por %s!!\n\n", enemy->nome);
			attron(COLOR_PAIR(3));
			printw("(Pressione alguma tecla para sair)\n");
			attron(COLOR_PAIR(1));
			getch();
			return 0;
		}
	}
	
	else
		printw("Voce desviou do ataque do inimigo!\n");	
	
	return 1;	
}

/* Verifica se o player esta sendo visto pelo inimigo e faz com que ele seja seguido */
void playerNear(Nivel *nivel, Enemy *enemy, int playerX, int playerY, int indice){

	int x, y, radius, i, j, stop = 0;

	y = (*enemy).y;
	x = (*enemy).x;
		
	for(i = 0; (i < (*nivel).tamI); i++)
		for(j = 0; (j < (*nivel).tamJ); j++)
			(*nivel).mapa[i][j].shown = 0;

	(*nivel).mapa[y][x].shown = 1;

	/* Verifica os espacos que o inimigo consegue ver */
	for(radius = 1; radius < 4; radius++){

		for(i = (*enemy).y - radius; (i <= (*enemy).y + radius); i++){
			for(j = (*enemy).x - radius; (j <= (*enemy).x + radius); j++){

				if(((i >= 0) && (i < (*nivel).tamI)) && ((j >= 0) && (j < (*nivel).tamJ))){

					if((*nivel).mapa[i][j].shown == radius){

						if(i - 1 >= 0)
							if((*nivel).mapa[i - 1][j].wall == 0)
	                       		(*nivel).mapa[i - 1][j].shown = radius + 1; 
		              
	                    if(i + 1 < (*nivel).tamI)
	                    	if((*nivel).mapa[i + 1][j].wall == 0)
	                        	(*nivel).mapa[i + 1][j].shown = radius + 1;

	                    if(j - 1 >= 0)
	                    	if((*nivel).mapa[i][j - 1].wall == 0)
	                        	(*nivel).mapa[i][j - 1].shown = radius + 1;

	                    if(j + 1 < (*nivel).tamJ)
	                    	if((*nivel).mapa[i][j + 1].wall == 0)
	                        	(*nivel).mapa[i][j + 1].shown = radius + 1;
	                }	
				}
			}
		}
	}					

	/* Verifica se o player esta visivel e proximo */
	for(i = (*enemy).y - 5; (i <= (*enemy).y + 5) && (!stop); i++){
		for(j = (*enemy).x - 5; (j <= (*enemy).x + 5) && (!stop); j++){

			if(((i >= 0) && (i < (*nivel).tamI)) && ((j >= 0) && (j < (*nivel).tamJ))){
				if(((*nivel).mapa[i][j].shown) && ((*nivel).mapa[i][j].player))
					stop = 1;
			}	
		}
	}
		
	/* Se estiver, segue-o, caso contrario continua parado */
	if(stop == 1){
		if((playerY < y) && ((*nivel).mapa[y - 1][x].used == 0)){
			(*enemy).y--;
			(*nivel).mapa[y][x].used = 0;
			(*nivel).mapa[y][x].enemyIndice = -1;

			(*nivel).mapa[y - 1][x].used = 1;
			(*nivel).mapa[y - 1][x].enemyIndice = indice;
		}

		else if((playerY > y) && ((*nivel).mapa[y + 1][x].used == 0)){
			(*enemy).y++;
			(*nivel).mapa[y][x].used = 0;
			(*nivel).mapa[y][x].enemyIndice = -1;

			(*nivel).mapa[y + 1][x].used = 1;
			(*nivel).mapa[y + 1][x].enemyIndice = indice;
		}

		else if((playerX < x) && ((*nivel).mapa[y][x - 1].used == 0)){
			(*enemy).x--;
			(*nivel).mapa[y][x].used = 0;
			(*nivel).mapa[y][x].enemyIndice = -1;

			(*nivel).mapa[y][x - 1].used = 1;
			(*nivel).mapa[y][x - 1].enemyIndice = indice;
		}

		else if ((*nivel).mapa[y][x + 1].used == 0){
			(*enemy).x++;
			(*nivel).mapa[y][x].used = 0;
			(*nivel).mapa[y][x].enemyIndice = -1;

			(*nivel).mapa[y][x + 1].used = 1;
			(*nivel).mapa[y][x + 1].enemyIndice = indice;
		}
	}
}

/* Funcao que movimenta os inimigos */
int enemyAction(Player *player, Nivel *nivel, Enemy *enemies){

	int i, x, y, flag;

	srand(time(NULL));

	/* Para cada inimigo */
	for(i = 0; i < (*nivel).inimigos; i++){
		if(enemies[i].hp > 0){
			x = enemies[i].x;
			y = enemies[i].y;
			flag = 1;

			/* Verifica se o player esta em uma regiao adjacente e o ataca */
			if((*nivel).mapa[y + 1][x].player){
				flag = 0;

				if(enemyAttack(player, &enemies[i]) == 0)
					return 0;
			}

			else if((flag) && ((*nivel).mapa[y - 1][x].player)){
				flag = 0;

				if(enemyAttack(player, &enemies[i]) == 0)
					return 0;
			}

			else if((flag) && ((*nivel).mapa[y][x - 1].player)){
				flag = 0;

				if(enemyAttack(player, &enemies[i]) == 0)
					return 0;
			}	

			else if((flag) && ((*nivel).mapa[y][x + 1].player)){
				flag = 0;

				if(enemyAttack(player, &enemies[i]) == 0)
					return 0;
			}

			/* Caso nao esteja, movimenta o inimigo */
			if(flag)
				playerNear(nivel, &enemies[i], (*player).x, (*player).y, i);	
		}	
	}	

	return 1;
}

/* Funcao que executa o ataque do player */
void combate(Player *player, Enemy *enemy, Map *position){

	int damageDone, attack, defense, critChance;

	/* Calcula o dano causado e o dano infringido, verifica se o inimigo morreu e *
	 * atualiza as posicoes no mapa de acordo com o resultado */
	srand(time(NULL));

	if(rand() % 100 > 30 * (*player).dext / 100){

		if(rand() % 2)
			attack = ceil((*player).attack + (0.2 * (*player).attack * (rand() % 100) / (double) 100)); 

		else 
			attack = ceil((*player).attack - (0.1 * (*player).attack * (rand() % 100) / (double) 100)); 

		critChance = 5 + (*player).dext / 5;

		srand(time(NULL));

		if(critChance >= rand() % 101){
			printw("Dano critico! ");
			attack += attack * 0.5;
		}

		defense = ceil((80 * (*enemy).defense + ((rand() % 21) * (*enemy).defense)) / (double) 100);

		damageDone = attack - defense;

		if(damageDone <= 0)
			damageDone = 1;

		(*enemy).hp -= damageDone;
		printw("Voce causou a %s %d de dano!\n", (*enemy).nome, damageDone);

		if((*enemy).hp <= 0){
			printw("%s morreu!\n", (*enemy).nome);
			printw("Voce recebeu %d de XP!\n", (*enemy).givenXP);
			(*position).enemyIndice = -1;
			(*position).used = 0;

			dropCheck(enemy, position);
			verificaXP(player, (*enemy).givenXP);

			if(enemy->indice == 0){
				attron(COLOR_PAIR(2));
				printw("O Guardiao do Nivel %d foi eliminado. Voce agora possui a chave para o proximo nivel!\n", player->atual);
				attron(COLOR_PAIR(1));
				player->chave = 1;
			}

		}

		if((*enemy).hp > 0)
			printw("%s possui %d de vida!\n", (*enemy).nome, (*enemy).hp);
	}
	
	else
		printw("%s desviou de seu ataque!\n", (*enemy).nome);
}