/* Author: ETA Team *
 * Last Modification: 07/20/2015 by Foo */

#include <stdio.h>
#include <stdlib.h>


/* Define o struct do mapa */
typedef struct {

	int wall, player, used, enemyIndice, itemIndice;

} Map;

typedef struct{

	int nivel, inimigos, tamI, tamJ;
	Map mapa[100][100]; 
} Nivel;


int main (){

	int i, j, count, recebe, n, m, mapas, aux;
	char comand;
	Nivel *nivel;	
	FILE *arq;

	printf("Quantos mapas vc criara?\n");
	scanf("%d", &mapas);

	nivel = malloc(mapas * sizeof(Nivel));

	for(count = 0; count < mapas; count++){
		printf("Insira o tamanho do mapa m x n:\n");
		scanf("%d %d", &m, &n);

		printf("Insira o nivel do mapa:\n");
		scanf("%d", &recebe);

		nivel[count].nivel = recebe;;
		nivel[count].tamI = m;
		nivel[count].tamJ = n;

		aux = 0;
 
		printf("\n\nPara cada posicao voce deve digitar 0, 1, 2 ou 3, onde:\n0 - posicao vazia;\n1 - muro;\n2 - inimigo;\n3 - item");
		printf(" (pressione enter, pois sera solicitado o indice do item na database!)\n\n\n");

		for(i = 0; i < m; i++){
			for(j = 0; j < n; j++){
				scanf("%d", &recebe);

				if(!recebe){
					nivel[count].mapa[i][j].used = 0;
					nivel[count].mapa[i][j].enemyIndice = -1;
					nivel[count].mapa[i][j].itemIndice = -1;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 0;
				}

				else if(recebe == 1){
					nivel[count].mapa[i][j].used = 1;
					nivel[count].mapa[i][j].enemyIndice = -1;
					nivel[count].mapa[i][j].itemIndice = -1;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 1;
				}

				else if(recebe == 2){
					nivel[count].mapa[i][j].used = 0;
					nivel[count].mapa[i][j].enemyIndice = nivel[count].nivel;
					nivel[count].mapa[i][j].itemIndice = -1;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 0;

					aux++;
				}

				else{

					printf("Insira o indice do item de acordo com a database:\n");
					scanf("%d", &recebe);

					nivel[count].mapa[i][j].used = 1;
					nivel[count].mapa[i][j].enemyIndice = -1;
					nivel[count].mapa[i][j].itemIndice = recebe;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 0;
				}
			}

			nivel[count].inimigos = aux;
		}

		printf("Mapa inserido:\n\n");

		for(i = 0; i < m; i++){
			for(j = 0; j < n; j++){
				if(nivel[count].mapa[i][j].player)
					printf("P ");

				else if(nivel[count].mapa[i][j].wall)
					printf("X ");

				else if(nivel[count].mapa[i][j].enemyIndice >= 0)
					printf("E ");

				else if(nivel[count].mapa[i][j].itemIndice >= 0)
					printf("I ");

				else
					printf("  ");
			}

			printf("\n");
		}

		printf("\n\nDeseja manter esse mapa? (y/n)\n");
		scanf(" %c", &comand);

		if(comand != 'y'){
			count--;
			printf("Mapa descartado. Digite outro mapa:\n\n\n");
		}

		else
			printf("Mapa salvo!\n\n\n");
	}

	arq = fopen("database2.bin", "r+b");

	if(arq == NULL){
		printf("Erro ao abrir database!\n");
		return 0;
	}

	fseek(arq, 0, SEEK_END);

	fwrite(nivel, mapas * sizeof(Nivel), 1, arq);

	fclose(arq);	

	printf("Sucesso moleque!!\n\n");
			
	return 0;
}