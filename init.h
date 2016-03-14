/* Author: ETA Team *
 * Last Modification: 03/11/2016 by Foo*/

/* Biblioteca que inicializa o jogo ou carrega a partida salva */

#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED 

#include <time.h>
#include <termios.h>
#include <curses.h>

/* Define stats do jogador */
#define BASE_HP 20
#define BASE_ATTACK 7
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10


/* Define quantidade de itens e tamanho da bag */
#define TAM_BAG 5


/* Struct de itens */
typedef struct {
    /* Tipos:
     * 1 - arma
     * 2 - armadura
     * 3 - pot */
    int indice, valor, tipo;
    char nome[51];
} Item;

 /* Define o struct do player */
typedef struct {

	int hp, level, attack, defense, XP, x, y, NextLevel, MaxHP, con, dext, str, pontos, chave, atual;
	Item weapon, gear;

} Player;

/* Define o struct de inimigos */
typedef struct {

	int hp, attack, defense, givenXP, x, y, dropItems[200], seen, indice;
	double dropRate;
	char nome[51];

} Enemy;

typedef struct {

	int wall, player, used, enemyIndice, itemIndice, quantItems, stairs, shown;

} Map;

typedef struct{

	int nivel, inimigos, tamI, tamJ;
	Map mapa[50][70]; 
} Nivel;

/* Struct da bag */
typedef struct {

	Item item;
	int quantidade, used;
} Bag;


/* Inicializa o jogador */
void playerInit(Player *player, Item *itens);

/* Inicializa a bag como vazia */
void bagInit(Bag *bag);

/* Detecta para cada inimigo a posicao deles no mapa */
void enemyPositions(Nivel nivel, Enemy *enemies);

/* Imprime o campo e os stats do jogador */
void print(Nivel nivel, Player controller, Enemy *enemies);

/* Verfica se existe e carrega partida salva */
int gameLoad(Player *player, Nivel *nivel, Enemy *enemies, Bag *bag);

#endif /* INIT_H_INCLUDED */