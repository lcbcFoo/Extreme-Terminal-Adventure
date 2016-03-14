/* Author: ETA Team *
 * Last Modification: 03/11/2016 by Foo*/

/* Biblioteca que executa comandos do jogo */

#ifndef COMAND_H_UNCLUDED
#define COMAND_H_UNCLUDED 


/* Inclui as bibliotecas que: *
 * inicializa o jogo, *
 * realiza as funcoes de combate *
 * executa comandos */

/* Define stats do jogador */
#define BASE_HP 20
#define BASE_ATTACK 7
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10


/* Define quantidade de itens e tamanho da bag */
#define TAM_BAG 5

/* Mostra os comandos permitidos para movimentar o personagem */
void comandList();

/* Salva o jogo */
int saveGame(Nivel nivel, Player *player, Enemy *enemies, Bag * bag);

/* Menu de comandos de controle sobre o jogo */
int menu(Nivel nivel, Player *player, Enemy *enemies, Bag *bag);

/* Funcao que usa pocoes */
void usaPot(Bag *bag, Player *player);

/* Funcao que printa o que tem dentro da mochila e permite que itens sejam usados */
void printBag(Bag *bag, Player *player, Nivel* nivel);

/* Funcao que pega o item de uma posicao do mapa */
void pegaItem(Player *player, Item item, Map *position, Bag *bag);

/* Desce na dungeon */
void nextNivel(Nivel* nivel, Player *player, Enemy** enemy);

/* Mostra os stats do jogador */
void showStats(Player *player);

/* Executa comandos de controle de personagem ou acessa o menu */
int executeComand(char comand, Player *player, Nivel *nivel, Enemy *enemies, Bag *bag, Item *itens);

#endif /* COMAND_H_UNCLUDED */