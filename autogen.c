/* Author: ETA Team *
 * Last Modification: 03/11/2016 by Foo*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <math.h>
#include <curses.h>
#include "init.h"
#include "autogen.h"


/* Checks if distance between player and boss is OK */
int distancia(int x1, int y1, int x2, int y2, int area){

    double calc = sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));

    if(2 * calc < sqrt(area))
        return 0;

    return 1;
}

/* Create new enemy */
Enemy createEnemy(int boss, int y, int x, int level, int indice){

    Enemy enemy;

    if(boss){
        char name[50] = "Guardiao do nivel";

        enemy.hp = (1 + level) * (BOSS_HP / 2);
        enemy.attack = (BOSS_ATK * (1 + level)) / 3;
        enemy.defense = (BOSS_DEFENSE * (1 + level)) / 1.5;
        enemy.givenXP = BOSS_XP * (1 + level) * 1.5;
        enemy.dropRate = BOSS_DROP_RATE;
        enemy.seen = 0;
        enemy.y = y;
        enemy.x = x;
        enemy.indice = 0;
        strcpy(enemy.nome, name);

        for(int i = 0; i < 200; i++)
            enemy.dropItems[i] = 1;

    }

    else{
        char name[50] = "Lacaio";

        do{
            enemy.hp = rand() % ((1 + level) * (BOSS_HP / 2));
        }while(enemy.hp < (1 + level) * (BOSS_HP / 6));

        do{
            enemy.attack = rand() % ((BOSS_ATK * (1 + level)) / 3);
        }while(enemy.attack < (BOSS_ATK * (1 + level)) / 9);

        do{
            enemy.defense = rand() % (BOSS_DEFENSE * (1 + level)) / 1.5;
        }while(enemy.defense < (BOSS_DEFENSE * (1 + level)) / 4.5);

        enemy.givenXP = BOSS_XP * (1 + level) / 2;

        do{
            enemy.dropRate = (rand() % 10 * BOSS_DROP_RATE) / 10;
        }while(enemy.dropRate < BOSS_DROP_RATE / 3);
        enemy.seen = 0;
        enemy.y = y;
        enemy.x = x;
        enemy.indice = indice;
        strcpy(enemy.nome, name);

        for(int i = 0; i < 200; i++)
            enemy.dropItems[i] = 1;
    }

    return enemy;
}

/* Gera um novo nivel */
Nivel* genNivel(int level, Player* player, Enemy** enemies){

    Nivel *nivel = malloc(sizeof(Nivel));

    /* Escolhe um tamanho aleatorio para o mapa */
    do{
        nivel->tamI = rand() % 26;
    }while(nivel->tamI < 19);

    do{
        nivel->tamJ = rand() % 45;
    }while(nivel->tamJ < 40);


    int tamI = nivel->tamI;
    int tamJ = nivel->tamJ;

    for(int i = 0; i < tamI; i++){
        for(int j = 0; j < tamJ; j++)
            nivel->mapa[i][j].shown = 0;
    }

    nivel->nivel = level;

    /* Gera o mapa aleatoriamente (apenas muros e espacos vazios) */
    genRoom(nivel);

    /* Inicializa as posicoes do mapa */
    for(int i = 0; i < tamI; i++){
        for(int j = 0; j < tamJ; j++){

            if( nivel->mapa[i][j].shown == 1){
                nivel->mapa[i][j].used = 1;
                nivel->mapa[i][j].enemyIndice = -1;
                nivel->mapa[i][j].itemIndice = -1;
                nivel->mapa[i][j].player = 0;
                nivel->mapa[i][j].wall = 1;
                nivel->mapa[i][j].stairs = 0;
                nivel->mapa[i][j].quantItems = 0;
            }

            else{
                nivel->mapa[i][j].used = 0;
                nivel->mapa[i][j].enemyIndice = -1;
                nivel->mapa[i][j].itemIndice = -1;
                nivel->mapa[i][j].player = 0;
                nivel->mapa[i][j].wall = 0;
                nivel->mapa[i][j].stairs = 0;
                nivel->mapa[i][j].quantItems = 0;
            }
        }
    }

    /* Places next stair */
    int stairsI, stairsJ;

    do{
        stairsI = rand() % tamI;
        stairsJ = rand() % tamJ;
    }while(nivel->mapa[stairsI][stairsJ].used == 1);

    nivel->mapa[stairsI][stairsJ].used = 1;
    nivel->mapa[stairsI][stairsJ].stairs = 1;

    for(int i = stairsI - 1; i <= stairsI + 1; i++){
        for(int j = stairsJ - 1; j <= stairsJ + 1; j++){
            if(nivel->mapa[i][j].wall == 1){
                if((i > 0) && (i < tamI - 1) && (j > 0) && (j < tamJ - 1)){
                    nivel->mapa[i][j].used = 0;
                    nivel->mapa[i][j].wall = 0;
                }
            }
        }
    }

    /* Create random enemies */
    genEnemies(nivel, enemies);

    /* Places boss */
    int flag = 0;

    if(stairsI - 1 > 0){
        nivel->mapa[stairsI - 1][stairsJ].enemyIndice = 0;
        nivel->mapa[stairsI - 1][stairsJ].used = 1;
        nivel->inimigos++;
        (*enemies)[0] = createEnemy(1, stairsI - 1, stairsJ, nivel->nivel, 0);
    }

    else
        flag = 1;

    if(flag == 1){
        if(stairsI + 1 < tamI - 1){
            nivel->mapa[stairsI + 1][stairsJ].enemyIndice = 0;
            nivel->mapa[stairsI + 1][stairsJ].used = 1;
            nivel->inimigos++;
            (*enemies)[0] = createEnemy(1, stairsI + 1, stairsJ, nivel->nivel, 0);
        }

        else
            flag = 2;
    }

    if(flag == 2){
        if(stairsJ - 1 > 0){
            nivel->mapa[stairsI][stairsJ - 1].enemyIndice = 0;
            nivel->mapa[stairsI][stairsJ - 1].used = 1;
            nivel->inimigos++;
            (*enemies)[0] = createEnemy(1, stairsI, stairsJ - 1, nivel->nivel, 0);
        }

        else
            flag = 3;
    }

    if(flag == 3){
        if(stairsJ + 1 < tamJ - 1){
            nivel->mapa[stairsI][stairsJ + 1].enemyIndice = 0;
            nivel->mapa[stairsI][stairsJ + 1].used = 1;
            nivel->inimigos++;
            (*enemies)[0] = createEnemy(1, stairsI, stairsJ + 1, nivel->nivel, 0);
        }
    }

    /* Places player and checks if he is not blocked */
    do{
        (*player).y = rand() % tamI;
        (*player).x = rand() % tamJ;

    }while((nivel->mapa[player->y][player->x].used == 1) ||
            (!distancia(player->y, player->x, stairsI, stairsJ, tamI * tamJ)));


    nivel->mapa[player->y][player->x].used = 1;
    nivel->mapa[player->y][player->x].player = 1;
    nivel->mapa[player->y][player->x].stairs = -1;

    for(int i = player->y - 1; i <= player->y + 1; i++){
        for(int j = player->x - 1; j <= player->x + 1; j++){
            if(nivel->mapa[i][j].wall == 1){
                if((i > 0) && (i < tamI - 1) && (j > 0) && (j < tamJ - 1)){
                    nivel->mapa[i][j].used = 0;
                    nivel->mapa[i][j].wall = 0;
                }
            }
        }
    }

    enemyPositions(nivel, (*enemies));
    return nivel;
}

/* Makes some crazy things to set map size */
void getRoomsStats(Nivel* nivel, int *rooms, int* maxSize){

    int tamI = nivel->tamI;
    int tamJ = nivel->tamJ;

    int maxRooms = (tamI * tamJ) / (tamI + tamJ);

    srand((unsigned)time(NULL));
    do{
        (*maxSize) = rand() % maxRooms;
    }while((*maxSize) < maxRooms / 2);

    (*rooms) = sqrt(((tamI * tamJ) / ((*maxSize) + 1)));
}

/* Create random enemies */
void genEnemies(Nivel* nivel, Enemy** enemies){

    /* Determines number of enemies */
    nivel->inimigos = 2 * (nivel->nivel + 1) / 5;

    if(nivel->inimigos == 0)
        nivel->inimigos = 1;

    else if(nivel->inimigos > 5)
        nivel->inimigos = 5;

    /* Creates and place them */
    (*enemies) = realloc((*enemies), (nivel->inimigos + 1) * sizeof(Enemy));

    for(int i = 1; i < nivel->inimigos + 1; i++){
        int x, y;

        do{
            y = rand() % nivel->tamI;
            x = rand() % nivel->tamJ;
        }while(nivel->mapa[y][x].used == 1);

        (*enemies)[i] = createEnemy(0, y, x, nivel->nivel, i);
        nivel->mapa[y][x].enemyIndice = i;
        nivel->mapa[y][x].used = 1;
    }
}

/* Opens closed areas */
void fixRoom(Nivel* nivel){

    int stop = 0, i, j;
    int tamI = nivel->tamI;
    int tamJ = nivel->tamJ;

    /* Detects a closed area */
    for(i = 1; (!stop) && (i < tamI - 1); i++){
        for(j = 0; (!stop) && (j < tamJ - 1); j++){
            if(nivel->mapa[i][j].shown == 2){
                i--;
                j--;
                stop = 1;
            }
        }
    }

    /* Tries to open it from above */
    if((nivel->mapa[i - 1][j].shown == 1) && (i > 1)){
        nivel->mapa[i - 1][j].shown = 0;
    }

    /* Or from below */
    while((i < tamI - 2) && (nivel->mapa[i + 1][j].shown != 1))
        i++;

    if(i < tamI - 2)
        nivel->mapa[i + 1][j].shown = 0;

    /* Or to the right*/
    else{
        while((j < tamJ - 2) && (nivel->mapa[i][j + 1].shown != 1))
            j++;

        if(j < tamJ - 2)
            nivel->mapa[i][j + 1].shown = 0;
    }

    testRoom(nivel);
}

/* Tests if the map is entirely connected */
void testRoom(Nivel* nivel){

    int walls = 0;
    int area;
    int tamI = nivel->tamI;
    int tamJ = nivel->tamJ;

    area = (tamI - 2) * (tamJ - 2);

    for(int i = 1; i < tamI - 1; i++){
        for(int j = 1; j < tamJ - 1; j++){
            if(nivel->mapa[i][j].shown == 1)
                walls++;
        }
    }

    /* Start counting open spaces than checks if the open spaces are
     * equal to map_area - walls */
    for(int i = 1; i < tamI - 1; i++){
        for(int j = 1; j < tamJ - 1; j++){
            if(nivel->mapa[i][j].shown != 1)
                nivel->mapa[i][j].shown = 2;
        }
    }

    int noMove = 0;
    int count = 3;
    int i, j;

    do{
        i = rand() % tamI;
        j = rand() % tamJ;

    }while(nivel->mapa[i][j].shown == 1);

    nivel->mapa[i][j].shown = 3;

    while(!noMove){
        noMove = 1;

        for(int i = 1; i < tamI - 1; i++){
            for(int j = 0; j < tamJ - 1; j++){
                if(nivel->mapa[i][j].shown == count){
                    if((nivel->mapa[i - 1][j].shown == 2) && (nivel->mapa[i - 1][j].shown != 1)){
                        noMove = 0;
                        nivel->mapa[i - 1][j].shown = count + 1;
                    }

                    if((nivel->mapa[i + 1][j].shown == 2) && (nivel->mapa[i + 1][j].shown != 1)){
                        noMove = 0;
                        nivel->mapa[i + 1][j].shown = count + 1;
                    }

                    if((nivel->mapa[i][j - 1].shown == 2) && (nivel->mapa[i][j - 1].shown != 1)){
                        noMove = 0;
                        nivel->mapa[i][j - 1].shown = count + 1;
                    }

                    if((nivel->mapa[i][j + 1].shown == 2) && (nivel->mapa[i][j + 1].shown != 1)){
                        noMove = 0;
                        nivel->mapa[i][j + 1].shown = count + 1;
                    }

                    nivel->mapa[i][j].shown++;
                }
            }
        }

        count++;
    }

    int freeSpace = 0;
    for(int i = 1; i < tamI - 1; i++){
        for(int j = 1; j < tamJ - 1; j++){
            if(nivel->mapa[i][j].shown == count)
                freeSpace++;
        }
    }

    /* If it is not, checks for closed areas */
    if(freeSpace + walls < area)
        fixRoom(nivel);
}

/* Generates a map */
void genRoom(Nivel* nivel){

    int rooms, maxSize;
    int tamI = nivel->tamI;
    int tamJ = nivel->tamJ;

    getRoomsStats(nivel, &rooms, &maxSize);

    /* Generates a sub-room. A sub-room cant be entirely closed */
    do{
        /* Picks the center of the room */
        int centerI = rand() % tamI;
        int centerJ = rand() % tamJ;

        /* Determines upper walls line */
        int up = centerI - (3 * (rand() % maxSize) / 2);

        if(up == centerI)
            up--;

        if(up < 0)
            up = 0;

        /* Determines lower walls line */
        int down = centerI + (3 * (rand() % maxSize) / 2);

        if(down == centerI)
            down++;

        if(down >= tamI)
            down= tamI - 1;

        /* Determines left walls collum */
        int left = centerJ - (3 * (rand() % maxSize) / 2);

        if(left == centerJ)
            left--;

        if(left < 0)
            left = 0;

        /* Determines right walls collum */
        int right = centerJ + (3 * (rand() % maxSize) / 2);

        if(right == centerJ)
            right++;

        if(right >= tamJ)
            right = tamJ - 1;

        /* These variables checks if some door was created in each side of the wall,
         * preventing that too many areas are initially closed */
        int door1 = 0;
        int door2 = 0;

        for(int j = left; j <= right; j++){

            if((j > 1) && (nivel->mapa[up][j].shown == 1)){
                door1 = 1;
                nivel->mapa[up][j - 1].shown = 2;
            }

            nivel->mapa[up][j].shown = 1;

            if((j > 1) && (nivel->mapa[down][j].shown == 1)){
                door2 = 1;
                nivel->mapa[down][j - 1].shown = 2;
            }

            nivel->mapa[down][j].shown = 1;
        }

        if(door1 == 0){
            int j;

            do{
                j = rand() % right;
            }while(j < left);

            nivel->mapa[up][j].shown = 2;
        }

        if(door2 == 0){
            int j;

            do{
                j = rand() % right;
            }while(j < left);

            nivel->mapa[down][j].shown = 2;
        }

        door1 = 0;
        door2 = 0;

        for(int i = up; i <= down; i++){

            if((i > 1) && (nivel->mapa[i][left].shown == 1)){
                door1 = 1;
                nivel->mapa[i - 1][left].shown = 2;
            }

            nivel->mapa[i][left].shown = 1;

            if((i > 1) && (nivel->mapa[i][right].shown == 1)){
                door2 = 1;
                nivel->mapa[i - 1][right].shown = 2;
            }

            nivel->mapa[i][right].shown = 1;
        }

        if(door1 == 0){
            int i;

            do{
                i = rand() % down;
            }while(i < up);

            nivel->mapa[i][left].shown = 2;
        }

        if(door2 == 0){
            int i;

            do{
                i = rand() % down;
            }while(i < up);

            nivel->mapa[i][right].shown = 2;
        }

        /* Puts walls on the extremities */
        for(int i = 0; i < tamJ; i++){
            nivel->mapa[tamI - 1][i].shown = 1;
            nivel->mapa[0][i].shown = 1;
        }

        for(int i = 0; i < tamI; i++){
            nivel->mapa[i][tamJ - 1].shown = 1;
            nivel->mapa[i][0].shown = 1;
        }

        rooms--;
    }while(rooms);

    testRoom(nivel);
}
