/* Author: ETA Team */


/* This is the heades for the functions that executes commands */

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
#include "autogen.h"


/* Show basic commands */
void comandList(){

	mvprintw(0,0,"Comandos disponiveis durante o jogo:");
	mvprintw(1,0,"w - anda para cima/ataca inimigo que esteja na posicao de cima no mapa;");
	mvprintw(2,0,"d - anda para direita/ataca inimigo que esteja na posicao da direita no mapa;");
	mvprintw(3,0,"s - anda para baixo/ataca inimigo que esteja na posicao de baixo no mapa;");
	mvprintw(4,0,"a - anda para esquerda/ataca inimigo que esteja na posicao da esquerda do mapa;");
	mvprintw(5,0,"b - abre o inventario;");
	mvprintw(6,0,"c - mostra os atributos do jogador e permite que o jogador distribua os pontos;");
	mvprintw(7,0,"- - acessa o menu do jogo;");
}

/* Saves the game */
int saveGame(Nivel nivel, Player *player, Enemy *enemies, Bag * bag){

	FILE *arq;

	arq = fopen("savedGame.bin", "wb");

	/* Saves current game situation */
	if(arq != NULL){

		if(!fwrite(&nivel, sizeof(Nivel), 1, arq)){
			fclose(arq);
			return 0;
		}

		if(!fwrite(player, sizeof(Player), 1, arq)){
			fclose(arq);
			return 0;
		}

 		fwrite(enemies, sizeof(Enemy), nivel.inimigos, arq);

		if(fwrite(bag, sizeof(Bag), TAM_BAG, arq) != TAM_BAG){
			fclose(arq);
			return 0;
		}

		fclose(arq);

		return 1;
	}

	return 0;
}

/* Game menu */
int menu(Nivel nivel, Player *player, Enemy *enemies, Bag *bag){

	int flag = 1;
	char get;

	attron(COLOR_PAIR(3));

	/* Lists possible commands */
	mvprintw(10,0,"==== Menu ====");
	attron(COLOR_PAIR(1));
	mvprintw(11,0,"Opcoes de comando:\n");
	mvprintw(12,0,"s - salvar o jogo\n");
	mvprintw(13,0,"r - retornar ao jogo\n");
	mvprintw(14,0,"q - deixar o jogo (tenha certeza que voce salvou o jogo antes!)\n");
	mvprintw(15,0,"l - lista de comandos de jogo\n");

	do{
		get = getch();

		/* Calls the function to save the game */
		if(get == 's'){
			if(saveGame(nivel, player, enemies, bag)){
				clear();
				mvprintw(5,0,"Jogo salvo!");
			}

			else
				mvprintw(5,0,"O jogo nao pode ser salvo, tente novamente\n\n");

			attron(COLOR_PAIR(1));
			return menu(nivel, player, enemies, bag);
		}

		/* Returns to map */
		else if(get == 'r'){
			clear();
			attron(COLOR_PAIR(1));
			return 1;
		}

		/* Closes game */
		else if(get == 'q'){
			clear();
			attron(COLOR_PAIR(1));
			return 0;
		}

		/* Show possible commands */
		else if(get == 'l'){
			clear();
			comandList();
			attron(COLOR_PAIR(1));
			return menu(nivel, player, enemies, bag);
		}

	}while(flag);

	attron(COLOR_PAIR(1));
	return 1;
}

/* Uses potion */
void usaPot(Bag *bag, Player *player){
	(*player).hp += (*bag).item.valor;

	if((*player).hp > (*player).MaxHP)
		(*player).hp = (*player).MaxHP;

	(*bag).quantidade--;

	if((*bag).quantidade == 0)
		(*bag).used = 0;
}

/* Prints bag */
void printBag(Bag *bag, Player *player, Nivel* nivel){

	int i, flag = 1;
	int used = 0;
	char get;
	Item aux;
	int getPosition = 45;

	attron(COLOR_PAIR(3));

	/* Prints equiped items */
	mvprintw(10,0,"Voce esta equipado com: \n\n\n");
	attron(COLOR_PAIR(4));
	printw("%s\n    Dano + %d\n\n", (*player).weapon.nome, (*player).weapon.valor);
	printw("%s\n    Armadura + %d", (*player).gear.nome, (*player).gear.valor);
	attron(COLOR_PAIR(3));


	/* Print bag items */
	mvprintw(20,0,"Itens na mochila:\n\n");

	for(i = 0; i < TAM_BAG; i++){
		if(bag[i].used){
			used++;
			printw("%d - %s (%d)\n", i + 1, bag[i].item.nome, bag[i].quantidade);

			if(bag[i].item.tipo == 1)
				printw("    Dano: %d\n\n", bag[i].item.valor);

			else if(bag[i].item.tipo == 2)
				printw("    Defesa: %d\n\n", bag[i].item.valor);

			else if(bag[i].item.tipo == 3)
				printw("    Recupera %d vida\n\n", bag[i].item.valor);
		}
	}

	if(!used){
		attron(COLOR_PAIR(2));
		printw("Inventario vazio!\n\n");
		attron(COLOR_PAIR(3));
	}

	printw("Comandos possiveis: \n\nDigite o indice do item que deseja usar\nr - retorna ao mapa\nd - Dropa algum item da mochila\n\n");

	flag = 1;

	/* Gets a command */
	do{
		move(getPosition, 0);
		get = getch();

		if(get == 'r'){
			clear();
			attron(COLOR_PAIR(1));
			flag = 0;
		}

		else if(get == 'd'){
			mvprintw(5,0,"\n");
			mvprintw(0,0,"Qual o indice do item que voce deseja dropar?\n");
			move(getPosition, 0);

			char index;

			index = getch();

			if((index - '1' >= 0) && (index - '1' < TAM_BAG) && (bag[index - '1'].used)){
				if(nivel->mapa[player->y - 1][player->x].used == 0){
					nivel->mapa[player->y - 1][player->x].used = 1;
					nivel->mapa[player->y - 1][player->x].quantItems = bag[index - '1'].quantidade;
					nivel->mapa[player->y - 1][player->x].itemIndice = bag[index - '1'].item.indice;
					bag[index - '1'].used = 0;
					attron(COLOR_PAIR(4));
					mvprintw(5,0,"O item foi dropado!\n");
					attron(COLOR_PAIR(1));
					printBag(bag, player, nivel);
					return;
				}

				else if(nivel->mapa[player->y + 1][player->x].used == 0){
					nivel->mapa[player->y + 1][player->x].used = 1;
					nivel->mapa[player->y + 1][player->x].quantItems = bag[index - '1'].quantidade;
					nivel->mapa[player->y + 1][player->x].itemIndice = bag[index - '1'].item.indice;
					bag[index - '1'].used = 0;
					attron(COLOR_PAIR(4));
					mvprintw(5,0,"O item foi dropado!\n");
					attron(COLOR_PAIR(1));
					printBag(bag, player, nivel);
					return;
				}

				else if(nivel->mapa[player->y][player->x - 1].used == 0){
					nivel->mapa[player->y][player->x - 1].used = 1;
					nivel->mapa[player->y][player->x - 1].quantItems = bag[index - '1'].quantidade;
					nivel->mapa[player->y][player->x - 1].itemIndice = bag[index - '1'].item.indice;
					bag[index - '1'].used = 0;
					attron(COLOR_PAIR(4));
					mvprintw(5,0,"O item foi dropado!\n");
					attron(COLOR_PAIR(1));
					printBag(bag, player, nivel);
					return;
				}

				else if(nivel->mapa[player->y][player->x + 1].used == 0){
					nivel->mapa[player->y][player->x + 1].used = 1;
					nivel->mapa[player->y][player->x + 1].quantItems = bag[index - '1'].quantidade;
					nivel->mapa[player->y][player->x + 1].itemIndice = bag[index - '1'].item.indice;
					bag[index - '1'].used = 0;
					attron(COLOR_PAIR(4));
					mvprintw(5,0,"O item foi dropado!\n");
					attron(COLOR_PAIR(1));
					printBag(bag, player, nivel);
					return;
				}

				else{
					attron(COLOR_PAIR(2));
					mvprintw(5,0,"Nao foi possivel dropar o item nessa posicao do mapa!\n");
					attron(COLOR_PAIR(1));
					printBag(bag, player, nivel);
					return;
				}
			}

			else{
				attron(COLOR_PAIR(2));
				mvprintw(5,0,"Nao ha nenhum item nessa posicao da mochila!\n\n");
				attron(COLOR_PAIR(1));
				printw("Escolha um novo comando\n");
				printBag(bag, player, nivel);
				return;
			}
		}

		else if((get - '1' >= 0) && (get - '1' < TAM_BAG) && (bag[get - '1'].used)){

			/* Uses pot */
			if(bag[get - '1'].item.tipo == 3){
				usaPot(&bag[get - '1'], player);
				attron(COLOR_PAIR(4));
				mvprintw(5,0,"Voce utilizou uma %s e recuperou %d de vida!\n", bag[get-'1'].item.nome,
																			bag[get-'1'].item.valor);
				attron(COLOR_PAIR(1));
				printBag(bag, player, nivel);
				return;
			}

			/* Changes weapon */
			else if(bag[get - '1'].item.tipo == 1){
				(*player).attack -= (*player).weapon.valor;
				aux = bag[get - '1'].item;
				bag[get - '1'].item = (*player).weapon;
				(*player).weapon = aux;
				(*player).attack += (*player).weapon.valor;
				attron(COLOR_PAIR(4));
				mvprintw(5,0,"Voce agora esta usando %s\n", (*player).weapon.nome);
				attron(COLOR_PAIR(1));
				printBag(bag, player, nivel);
				return;
			}

			/* Changes armor */
			else{
				(*player).defense -= (*player).gear.valor;
				aux = bag[get - '1'].item;
				bag[get -'1'].item = (*player).gear;
				(*player).gear = aux;
				(*player).defense += (*player).gear.valor;
				attron(COLOR_PAIR(4));
				mvprintw(5,0,"Voce agora esta usando %s\n", (*player).gear.nome);
				attron(COLOR_PAIR(1));
				printBag(bag, player, nivel);
				return;
			}
		}

	}while(flag);

	used = 0;

	for(int i = 0; i < TAM_BAG; i++){
		if(bag[i].used == 1){
			used = 1;
			break;
		}
	}

	if(used){
		for(int i = 0; i < TAM_BAG - 1; i++){
			if(bag[i].used == 0){
				flag = 0;

				for(int j = i; j < TAM_BAG - 1; j++)
					if(bag[j + 1].used){
						bag[j] = bag[j + 1];
						bag[j + 1].used = 0;
						flag = 1;
					}

				if(flag)
					i--;
			}
		}
	}
}

/* Takes item from map */
void takeItem(Player *player, Item item, Map *position, Bag *bag){

	int i, flag = 1;

	/* Checks if player has the item */
	for(i = 0; (i < TAM_BAG) && (flag); i++){
		if((bag[i].used) && (strcmp(bag[i].item.nome, item.nome) == 0)){
			if(item.tipo == 3){
				flag = 0;
				bag[i].quantidade++;
				bag[i].used = 1;
				(*position).itemIndice = -1;
				(*position).used = 0;
			}
		}
	}

	/* If not, checks if there is enough room in bag */
	for(i = 0; (i < TAM_BAG) && (flag); i++){
		if(bag[i].used == 0){
			bag[i].item = item;
			bag[i].quantidade = (*position).quantItems;
			bag[i].used = 1;
			flag = 0;
			(*position).used = 0;
			(*position).itemIndice = -1;
		}
	}

	/* Prints result */
	if(flag){
		clear();
		attron(COLOR_PAIR(4));
		mvprintw(0,0,"Seu inventario esta cheio!!\n");
		attron(COLOR_PAIR(1));
	}

	else{
		clear();
		attron(COLOR_PAIR(4));
		mvprintw(0,0,"Voce encontrou um item: %s\n", item.nome);
		attron(COLOR_PAIR(1));
	}
}

void nextNivel(Nivel *nivel, Player *player, Enemy** enemy){

	if(player->chave){
		(*nivel) = genNivel(nivel->nivel + 1, player, (*enemy));
		clear();
		attron(COLOR_PAIR(3));
		mvprintw(0, 0, "Voce desceu para o nivel %d da dungeon\n", nivel->nivel + 1);
		attron(COLOR_PAIR(1));
		player->chave = 0;
		player->atual++;
	}

	else{
		attron(COLOR_PAIR(2));
		printw("Voce nao possui a chave desse nivel, elimine o Boss para consegui-la!\n");
		attron(COLOR_PAIR(1));
	}
}

void showStats(Player *player){

	char get;

	attron(COLOR_PAIR(3));
	clear();
	mvprintw(0,0,"Existem 3 tipos de atributos: constituicao, destreza e forca. Voce get 5 pontos ao evoluir um nivel para distribuir entre esses atributos.\n");
	mvprintw(1,0,"Colocar pontos em constituicao te garante mais vida, em destreza te garante maior chance de se esquivar de ataques e de dar acertos criticos, ");
	mvprintw(2,0,"e em forca aumenta o seu poder de ataque e defesa.");
	mvprintw(5,0,"Seus atributos:\n\nConstituicao: %d\nDestreza: %d\nForca: %d", (*player).con, (*player).dext, (*player).str);
	mvprintw(11,0,"Voce possui %d pontos a serem distribuidos", (*player).pontos);
	mvprintw(14,0,"Comandos possiveis:\n1 - Adiciona 1 ponto em constituicao (caso voce possua pontos a serem distribuidos);\n2 - Adiciona 1 ponto em destreza (caso voce possua pontos a serem distribuidos)");
	mvprintw(17,0,"3 - Adiciona 1 ponto em forca (GASTA 5 PONTOS!) (caso voce possua pontos a serem distribuidos);\nr - retorna ao jogo\n\n");

	get = getch();

	if(get == 'r'){
		clear();
		attron(COLOR_PAIR(1));
		return;
	}

	if((*player).pontos){

		if(get == '1'){
			(*player).pontos--;
			(*player).con++;
			(*player).MaxHP += BASE_HP / 6;

		}

		else if(get == '2'){
			(*player).pontos--;
			(*player).dext++;
		}

		else if(((*player).pontos >= 5) && (get == '3')){
			(*player).pontos -= 5;
			(*player).str++;
			(*player).attack += BASE_ATTACK / 4;
			(*player).defense += BASE_DEF / 3;
		}
	}

	showStats(player);
}

/* Executes commands when in map */
int executeComand(char command, Player *player, Nivel *nivel, Enemy *enemies, Bag *bag, Item *itens){

	/* Opens menu */
	if(command == '-'){
		clear();
		return menu((*nivel), player, enemies, bag);
	}

	else if(command == 'c')
		showStats(player);

	else if(command == 'b'){
		clear();
		printBag(bag, player, nivel);
	}

	/* Combat/move commands*/
	else if(((*player).x > 0) && (command == 'a')) {

		/* Moving */
		if((*nivel).mapa[(*player).y][(*player).x - 1].used == 0){

			if((*nivel).mapa[(*player).y][(*player).x].stairs == 0)
				(*nivel).mapa[(*player).y][(*player).x].used = 0;

			(*nivel).mapa[(*player).y][(*player).x].player = 0;

			(*player).x--;
			(*nivel).mapa[(*player).y][(*player).x].player = 1;
			(*nivel).mapa[(*player).y][(*player).x].used = 1;
		}

		/* If there is an enemy in the position => combat */
		else if((*nivel).mapa[(*player).y][(*player).x - 1].enemyIndice != -1)
			combate(player, &enemies[(*nivel).mapa[(*player).y][(*player).x - 1].enemyIndice],
					&(*nivel).mapa[(*player).y][(*player).x - 1]);

		/* If there is an item in the position => take item */
		else if((*nivel).mapa[(*player).y][(*player).x - 1].itemIndice != -1)
			takeItem(player, itens[(*nivel).mapa[(*player).y][(*player).x - 1].itemIndice],
					&(*nivel).mapa[(*player).y][(*player).x - 1], bag);

		else if((*nivel).mapa[(*player).y][(*player).x - 1].stairs == 1)
			nextNivel(nivel, player, &enemies);

		/* Move enemies */
		return enemyAction(player, &(*nivel), enemies);
	}

	/* Same thing to d, w, s */
	else if(((*player).x < (*nivel).tamJ - 1) && (command == 'd')){

		if((*nivel).mapa[(*player).y][(*player).x + 1].used == 0){

			if((*nivel).mapa[(*player).y][(*player).x].stairs == 0)
				(*nivel).mapa[(*player).y][(*player).x].used = 0;

			(*nivel).mapa[(*player).y][(*player).x].player = 0;

			(*player).x++;
			(*nivel).mapa[(*player).y][(*player).x].player = 1;
			(*nivel).mapa[(*player).y][(*player).x].used = 1;
		}

		else if((*nivel).mapa[(*player).y][(*player).x + 1].enemyIndice != -1)
			combate(player, &enemies[(*nivel).mapa[(*player).y][(*player).x + 1].enemyIndice],
					&(*nivel).mapa[(*player).y][(*player).x + 1]);

		else if((*nivel).mapa[(*player).y][(*player).x + 1].itemIndice != -1)
			takeItem(player, itens[(*nivel).mapa[(*player).y][(*player).x + 1].itemIndice],
					&(*nivel).mapa[(*player).y][(*player).x + 1], bag);

		else if((*nivel).mapa[(*player).y][(*player).x + 1].stairs == 1)
			nextNivel(nivel, player, &enemies);

		return enemyAction(player, &(*nivel), enemies);
	}

	else if(((*player).y > 0) && (command == 'w')){

		if((*nivel).mapa[(*player).y - 1][(*player).x].used == 0){

			if((*nivel).mapa[(*player).y][(*player).x].stairs == 0)
				(*nivel).mapa[(*player).y][(*player).x].used = 0;

			(*nivel).mapa[(*player).y][(*player).x].player = 0;

			(*player).y--;
			(*nivel).mapa[(*player).y][(*player).x].player = 1;
			(*nivel).mapa[(*player).y][(*player).x].used = 1;
		}

		else if((*nivel).mapa[(*player).y - 1][(*player).x].enemyIndice != -1)
			combate(player, &enemies[(*nivel).mapa[(*player).y - 1][(*player).x].enemyIndice],
					&(*nivel).mapa[(*player).y - 1][(*player).x]);

		else if((*nivel).mapa[(*player).y - 1][(*player).x].itemIndice != -1)
			takeItem(player, itens[(*nivel).mapa[(*player).y - 1][(*player).x].itemIndice],
					&(*nivel).mapa[(*player).y - 1][(*player).x], bag);

		else if((*nivel).mapa[(*player).y - 1][(*player).x].stairs == 1)
			nextNivel(nivel, player, &enemies);

		return enemyAction(player, &(*nivel), enemies);
	}

	else if(((*player).y < (*nivel).tamI - 1) && (command == 's')){

		if((*nivel).mapa[(*player).y + 1][(*player).x].used == 0){

			if((*nivel).mapa[(*player).y][(*player).x].stairs == 0)
				(*nivel).mapa[(*player).y][(*player).x].used = 0;

			(*nivel).mapa[(*player).y][(*player).x].player = 0;

			(*player).y++;
			(*nivel).mapa[(*player).y][(*player).x].player = 1;
			(*nivel).mapa[(*player).y][(*player).x].used = 1;
		}

		else if((*nivel).mapa[(*player).y + 1][(*player).x].enemyIndice != -1)
			combate(player, &enemies[(*nivel).mapa[(*player).y + 1][(*player).x].enemyIndice],
					&(*nivel).mapa[(*player).y + 1][(*player).x]);

		else if((*nivel).mapa[(*player).y + 1][(*player).x].itemIndice != -1)
			takeItem(player, itens[(*nivel).mapa[(*player).y + 1][(*player).x].itemIndice],
					&(*nivel).mapa[(*player).y + 1][(*player).x], bag);

		else if((*nivel).mapa[(*player).y + 1][(*player).x].stairs == 1)
			nextNivel(nivel, player, &enemies);

		return enemyAction(player, &(*nivel), enemies);
	}

	return 1;
}
