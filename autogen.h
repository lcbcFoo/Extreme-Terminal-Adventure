#ifndef AUTOGEN_H_INCLUDED
#define AUTOGEN_H_INCLUDED

#define BOSS_HP 50
#define BOSS_ATK 13
#define BOSS_DEFENSE 5
#define BOSS_DROP_RATE 0.9
#define BOSS_XP 10


/* Generates new level */
Nivel genNivel(int level, Player* player, Enemy** enemies);

/* Generates room size attributes*/
void getRoomsStats(Nivel* nivel, int *maxSize, int* rooms);

/* Create random enemies */
void genEnemies(Nivel* nivel, Enemy** enemies);

/* Open closed areas */
void fixRoom(Nivel* nivel);

/* Tests if the map is entirely interconnected */
void testRoom(Nivel* nivel);

/* Generates new map */
void genRoom(Nivel* nivel);

#endif /* AUTOGEN_H_INCLUDED */
