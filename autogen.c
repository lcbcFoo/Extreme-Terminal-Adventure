/* Author: ETA Team *
 * Last Modification: 03/05/2015 by Foo*/

#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "autogen.h"
#include <math.h>


/* Calcula a distancia entre 2 pontos do mapa */
int distancia(int x1, int y1, int x2, int y2, int area){

	double calc = sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));

	if(2 * calc < sqrt(area))
		return 0;

	return 1;
}


/* Gera um novo nivel */
Nivel genNivel(int level, Player* player){

	Nivel nivel;

	/* Escolhe um tamanho aleatorio para o mapa */
	do{
		nivel.tamI = rand() % 36;
	}while(nivel.tamI < 26);

	do{
		nivel.tamJ = rand() % 36;
	}while(nivel.tamJ < 26);

	
	int tamI = nivel.tamI;
	int tamJ = nivel.tamJ;

	for(int i = 0; i < tamI; i++){
		for(int j = 0; j < tamJ; j++)
			nivel.mapa[i][j].shown = 0;
	}

	nivel.nivel = level;
	nivel.inimigos = 0;

	/* Gera o mapa aleatoriamente (apenas muros e espacos vazios) */
	genRoom(&nivel);

	/* Inicializa as posicoes do mapa */
	for(int i = 0; i < tamI; i++){
		for(int j = 0; j < tamJ; j++){

			if(nivel.mapa[i][j].shown == 1){
				nivel.mapa[i][j].used = 1;
				nivel.mapa[i][j].enemyIndice = -1;
				nivel.mapa[i][j].itemIndice = -1;
				nivel.mapa[i][j].player = 0;
				nivel.mapa[i][j].wall = 1;
				nivel.mapa[i][j].stairs = 0;
			}

			else{
				nivel.mapa[i][j].used = 0;
				nivel.mapa[i][j].enemyIndice = -1;
				nivel.mapa[i][j].itemIndice = -1;
				nivel.mapa[i][j].player = 0;
				nivel.mapa[i][j].wall = 0;
				nivel.mapa[i][j].stairs = 0;
			}
		}
	}	

	/* Posiciona a escada para o proximo nivel*/
	int stairsI, stairsJ;

	do{
		stairsI = rand() % tamI;
		stairsJ = rand() % tamJ;
	}while(nivel.mapa[stairsI][stairsJ].used == 1);

	nivel.mapa[stairsI][stairsJ].used = 1;
	nivel.mapa[stairsI][stairsJ].stairs = 1;

	/* Posiciona o jogador e garante que ele nao esta encostado em um muro (isso pode atrapalhar movimento em corredores) */
	do{
		(*player).y = rand() % tamI;
		(*player).x = rand() % tamJ;

	}while((nivel.mapa[player->y][player->x].used == 1) || (!distancia(player->y, player->x, stairsI, stairsJ, tamI * tamJ)));


	nivel.mapa[player->y][player->x].used = 1;
	nivel.mapa[player->y][player->x].player = 1;
	nivel.mapa[player->y][player->x].stairs = -1;

	for(int i = player->y - 1; i <= player->y + 1; i++){
		for(int j = player->x - 1; j <= player->x + 1; j++){
			if(nivel.mapa[i][j].wall == 1){
				if((i > 0) && (i < tamI - 1) && (j > 0) && (j < tamJ - 1)){
					nivel.mapa[i][j].used = 0;
					nivel.mapa[i][j].wall = 0;
				}
			}	
		}
	}

	return nivel;
}

/* Gera alguns elementos aleatorios para determinar o numero e tamanho de subsalas */
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

/* Abre uma subsala completamente fechada */
void fixRoom(Nivel* nivel){

	int stop = 0, i, j;
	int tamI = nivel->tamI;
	int tamJ = nivel->tamJ;

	/* Detecta alguma subsala fechada */
	for(i = 1; (!stop) && (i < tamI - 1); i++){
		for(j = 0; (!stop) && (j < tamJ - 1); j++){
			if(nivel->mapa[i][j].shown == 2){
				i--;
				j--;
				stop = 1;
			}
		}
	}

	/* Verifica se eh possivel abri-la para cima */
	if((nivel->mapa[i - 1][j].shown == 1) && (i > 1)){
		nivel->mapa[i - 1][j].shown = 0;
		return testRoom(nivel);
	}

	/* Ou para baixo */
	while((i < tamI - 2) && (nivel->mapa[i + 1][j].shown != 1))
		i++;

	if(i < tamI - 2)
		nivel->mapa[i + 1][j].shown = 0;

	/* Ou a abre para a direita */
	else{
		while((j < tamJ - 2) && (nivel->mapa[i][j + 1].shown != 1))
			j++;

		nivel->mapa[i][j + 1].shown = 0;
	}

	return testRoom(nivel);
}

/* Testa se o mapa esta totalmente interligado */
void testRoom(Nivel* nivel){

	int walls = 0;
	int area;
	int tamI = nivel->tamI;
	int tamJ = nivel->tamJ;

	/* Conta o numero de muros (tirando as bordas) e calcula a area (tambem tirando as bordas) */
	area = (tamI - 2) * (tamJ - 2);

	for(int i = 1; i < tamI - 1; i++){
		for(int j = 1; j < tamJ - 1; j++){
			if(nivel->mapa[i][j].shown == 1)
				walls++;
		}
	}

	/* Realiza uma busca em largura.
	 * Seleciona um ponto qualquer que nao seja muro *
	 * A cada iteracao visita os pontos vizinhos ao vizitado na ultima iteracao ateh nenhum ponto ser visitado */
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

	/* Verifica se o numero de espacos visitados + o total de muros eh igual a area total */
	int freeSpace = 0;

	for(int i = 1; i < tamI - 1; i++){
		for(int j = 1; j < tamJ - 1; j++){
			if(nivel->mapa[i][j].shown == count)
				freeSpace++;
		}
	}

	/* Se nao for, existe ao menos uma subsala nao interligada, chama a funcao que a interliga */
	if(freeSpace + walls < area)
		fixRoom(nivel);

}

/* Gera um mapa */
void genRoom(Nivel* nivel){

	int rooms, maxSize;
	int tamI = nivel->tamI;
	int tamJ = nivel->tamJ;

	/* Verifica quantas salas devemn ser geradas e variaveis de controle para seus tamanhos */
	getRoomsStats(nivel, &rooms, &maxSize);

	/* Gera um subsala a cada iteracao. Uma subsala nao deve ser fechada */
	do{
		/* Determina o centro da subsala */
		int centerI = rand() % tamI;
		int centerJ = rand() % tamJ;

		/* Determina a linha dos muros superiores */
		int up = centerI - (3 * (rand() % maxSize) / 2);

		if(up == centerI)
			up--;

		if(up < 0)
			up = 0;

		/* Determina alinha dos muros inferiores */
		int down = centerI + (3 * (rand() % maxSize) / 2);

		if(down == centerI)
			down++;

		if(down >= tamI)
			down= tamI - 1;

		/* Determina a coluna dos muros esquerdos */
		int left = centerJ - (3 * (rand() % maxSize) / 2);

		if(left == centerJ)
			left--;

		if(left < 0)
			left = 0;

		/* Determina acoluna dos muros direitos */
		int right = centerJ + (3 * (rand() % maxSize) / 2);

		if(right == centerJ)
			right++;

		if(right >= tamJ)
			right = tamJ - 1;

		/* Variaveis de controle que verificam se ao menos alguma porta foi criada em cada lado dos muros *
		 * Caso negativo, alguma posicao do lado do muro que nao recebeu uma porta eh aberta *
		 * Isso diminui o numero de subsalas fechadas e reduz significativamente o numero de chamadas da funcao de teste */
		int door1 = 0;
		int door2 = 0;

		/* Para os muros verticais */
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

		/* Idem para os horizontais */
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

		/* Cerca os extremos do mapa com muros */
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