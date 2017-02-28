#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void testRoom(int** matrice, int tamI, int tamJ);

int** matriceInit(int* tamI, int* tamJ){

	do{
		(*tamI) = rand() % 36;
	}while((*tamI) < 26);

	do{
		(*tamJ) = rand() % 36;
	}while((*tamJ) < 26);


	int** matrice = malloc((*tamI) * sizeof(int*));

	for(int i = 0; i < (*tamI); i++)
		matrice[i] = calloc((*tamJ), sizeof(int));

	return matrice;
}

void getRoomsStats(int** matrice, int tamI, int tamJ, int *maxSize, int* rooms){

	int maxRooms = (tamI * tamJ) / (tamI + tamJ);

	srand((unsigned)time(NULL));
	do{
		(*rooms) = rand() % maxRooms;
	}while((*rooms) < maxRooms / 2);

	(*maxSize) = sqrt(((tamI * tamJ) / ((*rooms) + 1)));
}


void fixRoom(int** matrice, int tamI, int tamJ){

	int stop = 0, i, j;

	for(i = 1; (!stop) && (i < tamI - 1); i++){
		for(j = 0; (!stop) && (j < tamJ - 1); j++){
			if(matrice[i][j] == 2){
				i--;
				j--;
				stop = 1;
			}
		}
	}

	if((matrice[i - 1][j] == 1) && (i > 1)){
		matrice[i - 1][j] = 0;
		testRoom(matrice, tamI, tamJ);
		return;
	}

	while((i < tamI - 2) && (matrice[i + 1][j] != 1))
		i++;

	if(i < tamI - 2)
		matrice[i + 1][j] = 0;

	else{
		while((j < tamJ - 2) && (matrice[i][j + 1] != 1))
			j++;

		matrice[i][j + 1] = 0;
	}

	testRoom(matrice, tamI, tamJ);
}

void testRoom(int** matrice, int tamI, int tamJ){

	int walls = 0;
	int area;

	area = (tamI - 2) * (tamJ - 2);

	for(int i = 1; i < tamI - 1; i++){
		for(int j = 1; j < tamJ - 1; j++){
			if(matrice[i][j] == 1)
				walls++;
		}
	}

	for(int i = 1; i < tamI - 1; i++){
		for(int j = 1; j < tamJ - 1; j++){
			if(matrice[i][j] != 1)
				matrice[i][j] = 2;
		}
	}

	int noMove = 0;
	int count = 3;
	int i, j;

	do{
		i = rand() % tamI;
		j = rand() % tamJ;

	}while(matrice[i][j] == 1);

	matrice[i][j] = 3;

	while(!noMove){
		noMove = 1;

		for(int i = 1; i < tamI - 1; i++){
			for(int j = 0; j < tamJ - 1; j++){
				if(matrice[i][j] == count){
					if((matrice[i - 1][j] == 2) && (matrice[i - 1][j] != 1)){
						noMove = 0;
						matrice[i - 1][j] = count + 1;
					}

					if((matrice[i + 1][j] == 2) && (matrice[i + 1][j] != 1)){
						noMove = 0;
						matrice[i + 1][j] = count + 1;
					}

					if((matrice[i][j - 1] == 2) && (matrice[i][j - 1] != 1)){
						noMove = 0;
						matrice[i][j - 1] = count + 1;
					}

					if((matrice[i][j + 1] == 2) && (matrice[i][j + 1] != 1)){
						noMove = 0;
						matrice[i][j + 1] = count + 1;
					}

					matrice[i][j]++;
				}
			}
		}

		count++;
	}

	int freeSpace = 0;

	for(int i = 1; i < tamI - 1; i++){
		for(int j = 1; j < tamJ - 1; j++){
			if(matrice[i][j] == count)
				freeSpace++;
		}
	}


	if(freeSpace + walls < area)
		fixRoom(matrice, tamI, tamJ);

}

void genRoom(int** matrice, int tamI, int tamJ){

	int rooms, maxSize;

	getRoomsStats(matrice, tamI, tamJ, &rooms, &maxSize);

	do{
		int centerI = rand() % tamI;
		int centerJ = rand() % tamJ;

		int up = centerI - (3 * (rand() % maxSize) / 2);

		if(up == centerI)
			up--;

		if(up < 0)
			up = 0;

		int down = centerI + (3 * (rand() % maxSize) / 2);

		if(down == centerI)
			down++;

		if(down >= tamI)
			down= tamI - 1;

		int left = centerJ - (3 * (rand() % maxSize) / 2);

		if(left == centerJ)
			left--;

		if(left < 0)
			left = 0;

		int right = centerJ + (3 * (rand() % maxSize) / 2);

		if(right == centerJ)
			right++;

		if(right >= tamJ)
			right = tamJ - 1;

		int door1 = 0;
		int door2 = 0;

		for(int j = left; j <= right; j++){

			if((j > 1) && (matrice[up][j] == 1)){
				door1 = 1;
				matrice[up][j - 1] = 2;
			}

			matrice[up][j] = 1;

			if((j > 1) && (matrice[down][j] == 1)){
				door2 = 1;
				matrice[down][j - 1] = 2;
			}

			matrice[down][j] = 1;
		}

		if(door1 == 0){
			int j;

			do{
				j = rand() % right;
			}while(j < left);

			matrice[up][j] = 2;
		}

		if(door2 == 0){
			int j;

			do{
				j = rand() % right;
			}while(j < left);

			matrice[down][j] = 2;
		}

		door1 = 0;
		door2 = 0;


		for(int i = up; i <= down; i++){

			if((i > 1) && (matrice[i][left] == 1)){
				door1 = 1;
				matrice[i - 1][left] = 2;
			}

			matrice[i][left] = 1;

			if((i > 1) && (matrice[i][right] == 1)){
				door2 = 1;
				matrice[i - 1][right] = 2;
			}

			matrice[i][right] = 1;
		}

		if(door1 == 0){
			int i;

			do{
				i = rand() % down;
			}while(i < up);

			matrice[i][left] = 2;
		}

		if(door2 == 0){
			int i;

			do{
				i = rand() % down;
			}while(i < up);

			matrice[i][right] = 2;
		}

		for(int i = 0; i < tamJ; i++){
			matrice[tamI - 1][i] = 1;
			matrice[0][i] = 1;
		}

		for(int i = 0; i < tamI; i++){
			matrice[i][tamJ - 1] = 1;
			matrice[i][0] = 1;
		}

		rooms--;
	}while(rooms);

	testRoom(matrice, tamI, tamJ);
}

int main(){

	srand((unsigned)time(NULL));

	int tamI;
	int tamJ;

	int** matrice = matriceInit(&tamI, &tamJ);

	genRoom(matrice, tamI, tamJ);

	for(int i = 0; i < tamI; i++){
		for(int j = 0; j < tamJ; j++){

			if(matrice[i][j] == 1)
				printf("X ");

			else
				printf("- ");
		}

		printf("\n");
	}

	for(int i = 0; i < tamI; i++)
		free(matrice[i]);

	free(matrice);

	return 0;
}
