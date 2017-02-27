/* Author: ETA Team */

/* This is the header of the functions responsible for initializing and running
 * the game */

#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED


/* Defines player stats, bag size and number of items in database */
#define BASE_HP 20
#define BASE_ATTACK 7
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10
#define TAM_BAG 5
#define BASE_HP 20
#define BASE_ATTACK 7
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10
#define TAM_BAG 5

int QUANT_ITEMS;


/* Items struct */
typedef struct {
    /* Tipos:
     * 1 - weapon
     * 2 - armor
     * 3 - pot */
    int indice, valor, tipo;
    char nome[51];
} Item;

 /* Player struct */
typedef struct {

	int hp, level, attack, defense, XP, x, y, NextLevel, MaxHP, con, dext, str, pontos, chave, atual;
	Item weapon, gear;

} Player;

/* Enemy struct */
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

/* Bag struct */
typedef struct {

	Item item;
	int quantidade, used;
} Bag;

/* Initializes everything and executes the game */
void startGame();

/* Initializes player */
void playerInit(Player *player, Item *items);

/* Initializes empty bag */
void bagInit(Bag *bag);

/* Checks enemies current positions */
void enemyPositions(Nivel nivel, Enemy *enemies);

/* Print current game situation */
void print(Nivel nivel, Player controller, Enemy *enemies);

/* Checks if there is a saved game */
int gameLoad(Player *player, Nivel *nivel, Enemy *enemies, Bag *bag);

#endif /* INIT_H_INCLUDED */
