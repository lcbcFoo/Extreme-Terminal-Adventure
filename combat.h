/* Author: ETA Team *
 * Last Modification: 02/28/2015 by Foo*/

#ifndef COMBAT_H_INCLUDED
#define COMBAT_H_INCLUDED 

/* Define stats do jogador */
#define BASE_HP 20
#define BASE_ATTACK 7
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10
#define QUANT_ITENS 6


/* Funcao que recebe XP e verifica se o personagem upou */
void verificaXP(Player *player, int xpGanho);

/* Verifica se algum item vai ser dropado */
void dropCheck(Enemy *enemy, Map *position);

/* Funcao que executa o ataque de inimigos */
int enemyAttack(Player *player, Enemy *enemy);

/* Deetecta se o jogador esta proximo de algum inimigo */
void playerNear(Nivel *nivel, Enemy *enemy, int playerX, int playerY, int indice);

/* Funcao que movimenta os inimigos */
int enemyAction(Player *player, Nivel *nivel, Enemy *enemies);

/* Funcao que executa o ataque do player */
void combate(Player *player, Enemy *enemy, Map *position);

#endif /* COMBAT_H_INCLUDED */