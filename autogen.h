/* Author: ETA Team *
 * Last Modification: 03/11/2016 by Foo*/

#ifndef AUTOGEN_H_INCLUDED
#define AUTOGEN_H_INCLUDED 

/* Gera um novo nivel */
Nivel genNivel(int level, Player* player);

/* Gera alguns elementos aleatorios para determinar o numero e tamanho de subsalas */
void getRoomsStats(Nivel* nivel, int *maxSize, int* rooms);

/* Abre uma subsala completamente fechada */
void fixRoom(Nivel* nivel);

/* Testa se o mapa esta totalmente interligado */
void testRoom(Nivel* nivel);

/* Gera um mapa */
void genRoom(Nivel* nivel);

#endif /* AUTOGEN_H_INCLUDED */