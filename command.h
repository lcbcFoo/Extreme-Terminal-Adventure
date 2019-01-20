/* Author: ETA Team */

/* Header to the functions which executes game commands */

#ifndef COMAND_H_UNCLUDED
#define COMAND_H_UNCLUDED

#include "init.h"

/* Show basic commands */
void comandList();

/* Saves the game */
int saveGame(Nivel nivel, Player *player, Enemy *enemies, Bag * bag);
/* Game menu */
int menu(Nivel nivel, Player *player, Enemy *enemies, Bag *bag);

/* Uses potion */
void usaPot(Bag *bag, Player *player);

/* Prints bag */
void printBag(Bag *bag, Player *player, Nivel* nivel);

/* Takes item from map */
void takeItem(Player *player, Item item, Map *position, Bag *bag);

/* Generates next level */
void nextNivel(Nivel *nivel, Player *player, Enemy** enemy);

/* Show player stats */
void showStats(Player *player);

/* Executes commands when in map */
int executeComand(char command, Player *player, Nivel *nivel, Enemy *enemies, Bag *bag, Item *itens);

#endif /* COMAND_H_UNCLUDED */
