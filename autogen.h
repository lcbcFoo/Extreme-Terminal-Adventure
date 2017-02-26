#ifndef AUTOGEN_H_INCLUDED
#define AUTOGEN_H_INCLUDED

/* Generates new level */
Nivel genNivel(int level, Player* player, Enemy* enemies);

/* Generates room size attributes*/
void getRoomsStats(Nivel* nivel, int *maxSize, int* rooms);

/* Open closed areas */
void fixRoom(Nivel* nivel);

/* Tests if the map is entirely interconnected */
void testRoom(Nivel* nivel);

/* Generates new map */
void genRoom(Nivel* nivel);

#endif /* AUTOGEN_H_INCLUDED */
