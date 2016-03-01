/* Author: ETA Team *
 * Last Modification: 02/28/2015 by Foo*/

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
int saveGame(Nivel *niveis, Player *player, Enemy **enemies, Bag * bag, int n);

/* Menu de comandos de controle sobre o jogo */
int menu(Nivel *niveis, Player *player, Enemy **enemies, Bag *bag, int n);

/* Funcao que usa pocoes */
void usaPot(Bag *bag, Player *player);

/* Funcao que printa o que tem dentro da mochila e permite que itens sejam usados */
void printBag(Bag *bag, Player *player);

/* Funcao que pega o item de uma posicao do mapa */
void pegaItem(Player *player, Item item, Map *position, Bag *bag);

/* Upa o jogador */
void nextNivel(Nivel *niveis, Player *player);

/* Mostra os stats do jogador */
void showStats(Player *player);

/* Executa comandos de controle de personagem ou acessa o menu */
int executeComand(char comand, Player *player, Nivel *niveis, Enemy **enemies, Bag *bag, Item *itens, int n);

#endif /* COMAND_H_UNCLUDED */