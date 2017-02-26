/* Author: ETA Team *
 * Last Modification: 03/11/2016 by Foo*/

#ifndef COMBAT_H_INCLUDED
#define COMBAT_H_INCLUDED

/* Receives XP */
void verificaXP(Player *player, int xpGanho);

/* Check if an item was dropped */
void dropCheck(Enemy *enemy, Map *position);

/* Enemy atacks! */
int enemyAttack(Player *player, Enemy *enemy);

/* Checks if enemy can see the player and make it follow the player */
void playerNear(Nivel *nivel, Enemy *enemy, int playerX, int playerY, int indice);

/* Move enemies */
int enemyAction(Player *player, Nivel *nivel, Enemy *enemies);

/* Player attacks! */
void combate(Player *player, Enemy *enemy, Map *position);


#endif /* COMBAT_H_INCLUDED */
