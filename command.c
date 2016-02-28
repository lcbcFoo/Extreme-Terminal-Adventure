/* Author: ETA Team *
 * Last Modification: 02/28/2015 by Foo*/


/* Biblioteca que executa comandos do jogo */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <math.h>
#include "init.h"
#include "command.h"
#include "combat.h"



/* Define stats do jogador */
#define BASE_HP 20
#define BASE_ATTACK 7
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10


/* Define quantidade de itens e tamanho da bag */
#define TAM_BAG 5
#define QUANT_ITENS 6


/* Mostra os comandos permitidos para movimentar o personagem */
void comandList(){

	printf("\n\nComandos disponiveis durante o jogo:\n");
	printf("w - anda para cima/ataca inimigo que esteja na posicao de cima no mapa;\n");
	printf("d - anda para direita/ataca inimigo que esteja na posicao da direita no mapa;\n");
	printf("s - anda para baixo/ataca inimigo que esteja na posicao de baixo no mapa;\n");
	printf("a - anda para esquerda/ataca inimigo que esteja na posicao da esquerda do mapa;\n");
	printf("b - abre o inventario;\n");
	printf("c - mostra os atributos do jogador e permite que o jogador distribua os pontos;\n");
	printf("- - acessa o menu do jogo;\n\n\n");
}

/* Salva o jogo */
int saveGame(Nivel *niveis, Player *player, Enemy **enemies, Bag * bag, int n){

	int i;
	FILE *arq;

	arq = fopen("data.bin", "wb");

	/* Salva os elementos contidos no mapa, os stats do jogador e os inimigos *
	 * existentes no arquivo que salva o jogo */
	if(arq != NULL){

		if(fwrite(niveis, sizeof(Nivel), n, arq) != n){
			fclose(arq);
			return 0;
		}	

		if(!fwrite(player, sizeof(Player), 1, arq)){
			fclose(arq);
			return 0;
		}

		for (i = 0; i < n; i++)
   			fwrite(enemies[i], sizeof(**enemies), niveis[i].inimigos, arq);

		if(fwrite(bag, sizeof(Bag), TAM_BAG, arq) != TAM_BAG){
			fclose(arq);
			return 0;
		}	

		fclose(arq);

		return 1;
	}

	return 0;
}

/* Menu de comandos de controle sobre o jogo */
int menu(Nivel *niveis, Player *player, Enemy **enemies, Bag *bag, int n){

	int flag = 1;
	char recebe;

	/* Lista os comandos disponiveis e salva executa o comando desejado pelo jogador */
	printf("\n\n==== Menu ====\n\nOpcoes de comando:\n");
	printf("s - salvar o jogo\n");
	printf("r - retornar ao jogo\n");
	printf("q - deixar o jogo (tenha certeza que voce salvou o jogo antes!)\n");
	printf("l - lista de comandos de jogo\n");

	/* Repete o loop ateh recebr um comando valido */
	do{
		recebe = getch();

		/* Chama a funcao que salva o jogo */
		if(recebe == 's'){
			if(saveGame(niveis, player, enemies, bag, n)){
				system("clear");
				printf("\n\nJogo salvo!\n\n");
			}	

			else
				printf("O jogo nao pode ser salvo, tente novamente\n\n");

			return menu(niveis, player, enemies, bag, n);
		}

		/* Retorna ao mapa */
		else if(recebe == 'r'){
			system("clear");
			return 1;
		}	

		/* Fecha o jogo */
		else if(recebe == 'q'){
			system("clear");
			return 0;
		}

		/* Mostra os comandos disponiveis */
		else if(recebe == 'l'){
			system("clear");
			comandList();
			return menu(niveis, player, enemies, bag, n);
		}

	}while(flag);

	return 1;
}

/* Funcao que usa pocoes */
void usaPot(Bag *bag, Player *player){

	/* Recupera a vida */
	(*player).hp += (*bag).item.valor;

	/* Nao permite que a vida maxima seja ultrapassada */
	if((*player).hp > (*player).MaxHP)
		(*player).hp = (*player).MaxHP;

	/* Retira uma pot da bag */
	(*bag).quantidade--;

	if((*bag).quantidade == 0)
		(*bag).used = 0;
}

/* Funcao que printa o que tem dentro da mochila e permite que itens sejam usados */
void printBag(Bag *bag, Player *player){

	int i, flag = 1;
	char recebe;
	Item aux;

	system("clear");

	/* Imprime os equipamentos utilizados no momento */
	printf("Voce esta equipado com: \n\n");
	printf("%s\n    Dano = %d\n\n", (*player).weapon.nome, (*player).weapon.valor);
	printf("%s\n    Armadura = %d\n\n\n\n", (*player).gear.nome, (*player).gear.valor);


	/* Imprime os itens na mochila */
	printf("Itens na mochila:\n\n");

	for(i = 0; i < TAM_BAG; i++){
		if(bag[i].used){
			flag = 0;
			printf("%d - %s (%d)\n", i + 1, bag[i].item.nome, bag[i].quantidade);

			if(bag[i].item.tipo == 1)
				printf("    Dano: %d\n\n", bag[i].item.valor);

			else if(bag[i].item.tipo == 2)
				printf("    Defesa: %d\n\n", bag[i].item.valor);

			else if(bag[i].item.tipo == 3)
				printf("    Recupera %d vida\n\n", bag[i].item.valor);
		}
	}

	if(flag)
		printf("Inventario vazio!!\n\n");

	printf("Comandos possiveis: \nDigite o indice do item que deseja usar\nr - retorna ao mapa\n\n");

	flag = 1;

	/* Recebe um comando de utilizacao de item ou para voltar ao mapa */
	do{
		recebe = getch();

		if(recebe == 'r'){
			system("clear");
			return;
		}

		else if((recebe - '1' >= 0) && (recebe - '1' < TAM_BAG) && (bag[recebe - '1'].used)){
			flag = 0;

			/* Usa pot */
			if(bag[recebe - '1'].item.tipo == 3){
				usaPot(&bag[recebe - '1'], player);
				system("clear");
				printf("Voce utilizou uma %s e recuperou %d de vida!\n\n\n", bag[recebe-'1'].item.nome,
																			bag[recebe-'1'].item.valor);
			}

			/* Troca de arma */
			else if(bag[recebe - '1'].item.tipo == 1){
				(*player).attack -= (*player).weapon.valor;
				aux = bag[recebe - '1'].item;
				bag[recebe - '1'].item = (*player).weapon;
				(*player).weapon = aux;
				(*player).attack += (*player).weapon.valor;
				system("clear");
				printf("Voce agora esta usando %s\n\n\n", (*player).weapon.nome);
			}

			/* Troca de armadura */
			else{
				(*player).defense -= (*player).gear.valor;
				aux = bag[recebe - '1'].item;
				bag[recebe -'1'].item = (*player).gear;
				(*player).gear = aux;
				(*player).defense += (*player).gear.valor;
				system("clear");
				printf("Voce agora esta usando %s\n\n\n", (*player).gear.nome);				
			}

		}

	}while(flag);
}

/* Funcao que pega o item de uma posicao do mapa */
void pegaItem(Player *player, Item item, Map *position, Bag *bag){

	int i, flag = 1;

	/* Verifica se voce ja tem aquele item */
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

	/* Caso voce ainda nao tenha, verifica se existe uma posicao vazia na bag */
	for(i = 0; (i < TAM_BAG) && (flag); i++){
		if(bag[i].used == 0){
			bag[i].item = item;
			bag[i].quantidade = 1;
			bag[i].used = 1;
			flag = 0;
			(*position).used = 0;
			(*position).itemIndice = -1;
		}
	}

	/* Imprime se vc pegou o item ou se sua bag tava cheia */
	if(flag){
		printf("Seu inventario esta cheio!!\n\n");
	}

	else
		printf("Voce encontrou um item: %s\n\n", item.nome);
}

void nextNivel(Nivel *niveis, Player *player){

	int i, j, atual, flag = 0;


	(*player).nivelAtual++;
	printf("Voce desceu para o nivel %d da dungeon\n", (*player).nivelAtual + 1);
	atual = (*player).nivelAtual;

	for(i = 0; (i < niveis[atual].tamI) && (!flag); i++)
		for(j = 0; (j < niveis[atual].tamJ) && (!flag); j++)
			if(niveis[atual].mapa[i][j].stairs == -1){
				flag = 1;
				niveis[atual].mapa[i][j].player = 1;
				(*player).x = j;
				(*player).y = i;
			}

}

void returnNivel(Nivel *niveis, Player *player){

	int i, j, atual, flag = 0;

	(*player).nivelAtual--;
	printf("Voce subiu para o nivel %d da dungeon\n", (*player).nivelAtual + 1);
	atual = (*player).nivelAtual;

	for(i = 0; (i < niveis[atual].tamI) && (!flag); i++)
		for(j = 0; (j < niveis[atual].tamJ) && (!flag); j++)
			if(niveis[atual].mapa[i][j].stairs == 1){
				flag = 1;
				niveis[atual].mapa[i][j].player = 1;
				(*player).x = j;
				(*player).y = i;
			}
}

void showStats(Player *player){

	char recebe;

	system("clear");
	printf("Existem 3 tipos de atributos: constituicao, destreza e forca. Voce recebe 5 pontos ao evoluir um nivel para distribuir entre esses atributos.\n");
	printf("Colocar pontos em constituicao te garante mais vida, em destreza te garante maior chance de se esquivar de ataques e de dar acertos criticos, ");
	printf("e em forca aumenta o seu poder de ataque e defesa.\n\n");
	printf("Seus atributos:\n\nConstituicao: %d\nDestreza: %d\nForca: %d\n\n", (*player).con, (*player).dext, (*player).str);
	printf("Voce possui %d pontos a serem distribuidos\n\n\n", (*player).pontos);
	printf("Comandos possiveis:\n1 - Adiciona 1 ponto em constituicao (caso voce possua pontos a serem distribuidos);\n2 - Adiciona 1 ponto em destreza (caso voce possua pontos a serem distribuidos)");
	printf("\n3 - Adiciona 1 ponto em forca (GASTA 5 PONTOS!) (caso voce possua pontos a serem distribuidos);\nr - retorna ao jogo\n\n");

	recebe = getch();

	if(recebe == 'r'){
		system("clear");
		return;
	}

	if((*player).pontos){
		
		if(recebe == '1'){
			(*player).pontos--;
			(*player).con++;
			(*player).MaxHP += BASE_HP / 6;

		}

		else if(recebe == '2'){
			(*player).pontos--;
			(*player).dext++;
		}

		else if(((*player).pontos >= 5) && (recebe == '3')){
			(*player).pontos -= 5;
			(*player).str++;
			(*player).attack += BASE_ATTACK / 4;
			(*player).defense += BASE_DEF / 3;
		}
	}	

	showStats(player);
}

/* Executa comandos de controle de personagem ou acessa o menu */
int executeComand(char command, Player *player, Nivel *niveis, Enemy **enemies, Bag *bag, Item *itens, int n){

	int atual;

	atual = (*player).nivelAtual;

	/* Recebe os comandos durante o jogo e os executa */
	/* Abre o menu */
	if(command == '-')
		return menu(niveis, player, enemies, bag, n);

	else if(command == 'c')
		showStats(player);

	else if(command == 'b')
		printBag(bag, player);

	/* Comandos de movimento / combate */
	else if(((*player).x > 0) && (command == 'a')) {

		/* Esse eh o caso de movimento */
		if(niveis[atual].mapa[(*player).y][(*player).x - 1].used == 0){

			if(niveis[atual].mapa[(*player).y][(*player).x].stairs == 0)
				niveis[atual].mapa[(*player).y][(*player).x].used = 0;

			niveis[atual].mapa[(*player).y][(*player).x].player = 0;

			(*player).x--;
			niveis[atual].mapa[(*player).y][(*player).x].player = 1;
			niveis[atual].mapa[(*player).y][(*player).x].used = 1;
		}
		
		/* Esse eh o caso de combate, chama a funcao combate, a funcao de combate receb o endereco de 1 inimigo na matriz de inimigos */
		else if(niveis[atual].mapa[(*player).y][(*player).x - 1].enemyIndice != -1)
			combate(player, &enemies[(*player).nivelAtual][niveis[atual].mapa[(*player).y][(*player).x - 1].enemyIndice], 
					&niveis[atual].mapa[(*player).y][(*player).x - 1]);	

		/* Caso em que ha um item na posicao */
		else if(niveis[atual].mapa[(*player).y][(*player).x - 1].itemIndice != -1)
			pegaItem(player, itens[niveis[atual].mapa[(*player).y][(*player).x - 1].itemIndice], 
					&niveis[atual].mapa[(*player).y][(*player).x - 1], bag);

		else if(niveis[atual].mapa[(*player).y][(*player).x - 1].stairs == 1){
			niveis[atual].mapa[(*player).y][(*player).x].player = 0;
			niveis[atual].mapa[(*player).y][(*player).x].used = 0;
			nextNivel(niveis, player);
		}

		else if(niveis[atual].mapa[(*player).y][(*player).x - 1].stairs == -1){
			niveis[atual].mapa[(*player).y][(*player).x].player = 0;
			niveis[atual].mapa[(*player).y][(*player).x].used = 0;
			returnNivel(niveis, player);
		}

		/* Movimenta os inimigos e retorna se o jogador ainda esta vivo */
		return enemyAction(player, &niveis[atual], enemies[atual]);
	}

	/* Idem para os d, w, s */
	else if(((*player).x < niveis[atual].tamJ - 1) && (command == 'd')){

		if(niveis[atual].mapa[(*player).y][(*player).x + 1].used == 0){

			if(niveis[atual].mapa[(*player).y][(*player).x].stairs == 0)
				niveis[atual].mapa[(*player).y][(*player).x].used = 0;

			niveis[atual].mapa[(*player).y][(*player).x].player = 0;

			(*player).x++;
			niveis[atual].mapa[(*player).y][(*player).x].player = 1;
			niveis[atual].mapa[(*player).y][(*player).x].used = 1;
		}
		
		else if(niveis[atual].mapa[(*player).y][(*player).x + 1].enemyIndice != -1)
			combate(player, &enemies[(*player).nivelAtual][niveis[atual].mapa[(*player).y][(*player).x + 1].enemyIndice], 
					&niveis[atual].mapa[(*player).y][(*player).x + 1]);	

		/* Caso em que ha um item na posicao */
		else if(niveis[atual].mapa[(*player).y][(*player).x + 1].itemIndice != -1)
			pegaItem(player, itens[niveis[atual].mapa[(*player).y][(*player).x + 1].itemIndice], 
					&niveis[atual].mapa[(*player).y][(*player).x + 1], bag);

		else if(niveis[atual].mapa[(*player).y][(*player).x + 1].stairs == 1){
			niveis[atual].mapa[(*player).y][(*player).x].player = 0;
			niveis[atual].mapa[(*player).y][(*player).x].used = 0;
			nextNivel(niveis, player);
		}

		else if(niveis[atual].mapa[(*player).y][(*player).x + 1].stairs == -1){
			niveis[atual].mapa[(*player).y][(*player).x].player = 0;
			niveis[atual].mapa[(*player).y][(*player).x].used = 0;
			returnNivel(niveis, player);
		}

	
		return enemyAction(player, &niveis[atual], enemies[atual]);
	}

	else if(((*player).y > 0) && (command == 'w')){

		if(niveis[atual].mapa[(*player).y - 1][(*player).x].used == 0){

			if(niveis[atual].mapa[(*player).y][(*player).x].stairs == 0)
				niveis[atual].mapa[(*player).y][(*player).x].used = 0;

			niveis[atual].mapa[(*player).y][(*player).x].player = 0;

			(*player).y--;
			niveis[atual].mapa[(*player).y][(*player).x].player = 1;
			niveis[atual].mapa[(*player).y][(*player).x].used = 1;
		}
		
		else if(niveis[atual].mapa[(*player).y - 1][(*player).x].enemyIndice != -1)
			combate(player, &enemies[(*player).nivelAtual][niveis[atual].mapa[(*player).y - 1][(*player).x].enemyIndice], 
					&niveis[atual].mapa[(*player).y - 1][(*player).x]);
		
		/* Caso em que ha um item na posicao */
		else if(niveis[atual].mapa[(*player).y - 1][(*player).x].itemIndice != -1)
			pegaItem(player, itens[niveis[atual].mapa[(*player).y - 1][(*player).x].itemIndice], 
					&niveis[atual].mapa[(*player).y - 1][(*player).x], bag);

		else if(niveis[atual].mapa[(*player).y - 1][(*player).x].stairs == 1){
			niveis[atual].mapa[(*player).y][(*player).x].player = 0;
			niveis[atual].mapa[(*player).y][(*player).x].used = 0;
			nextNivel(niveis, player);
		}

		else if(niveis[atual].mapa[(*player).y - 1][(*player).x].stairs == -1){
			niveis[atual].mapa[(*player).y][(*player).x].player = 0;
			niveis[atual].mapa[(*player).y][(*player).x].used = 0;
			returnNivel(niveis, player);
		}


		return enemyAction(player, &niveis[atual], enemies[atual]);
	}
	
	else if(((*player).y < niveis[atual].tamI - 1) && (command == 's')){

		if(niveis[atual].mapa[(*player).y + 1][(*player).x].used == 0){

			if(niveis[atual].mapa[(*player).y][(*player).x].stairs == 0)
				niveis[atual].mapa[(*player).y][(*player).x].used = 0;

			niveis[atual].mapa[(*player).y][(*player).x].player = 0;

			(*player).y++;
			niveis[atual].mapa[(*player).y][(*player).x].player = 1;
			niveis[atual].mapa[(*player).y][(*player).x].used = 1;
		}
		
		else if(niveis[atual].mapa[(*player).y + 1][(*player).x].enemyIndice != -1)
			combate(player, &enemies[(*player).nivelAtual][niveis[atual].mapa[(*player).y + 1][(*player).x].enemyIndice], 
					&niveis[atual].mapa[(*player).y + 1][(*player).x]);

		/* Caso em que ha um item na posicao */
		else if(niveis[atual].mapa[(*player).y + 1][(*player).x].itemIndice != -1)
			pegaItem(player, itens[niveis[atual].mapa[(*player).y + 1][(*player).x].itemIndice], 
					&niveis[atual].mapa[(*player).y + 1][(*player).x], bag);

		else if(niveis[atual].mapa[(*player).y + 1][(*player).x].stairs == 1){
			niveis[atual].mapa[(*player).y][(*player).x].player = 0;
			niveis[atual].mapa[(*player).y][(*player).x].used = 0;
			nextNivel(niveis, player);
		}

		else if(niveis[atual].mapa[(*player).y + 1][(*player).x].stairs == -1){
			niveis[atual].mapa[(*player).y][(*player).x].player = 0;
			niveis[atual].mapa[(*player).y][(*player).x].used = 0;
			returnNivel(niveis, player);
		}

		return enemyAction(player, &niveis[atual], enemies[atual]);
	}

	return 1;
}