/* Author: ETA Team *
 * Last Modification: 07/22/2015 by Chams */

#include <stdio.h>
#include <stdlib.h>


/* Define o struct do mapa */
typedef struct {

	int wall, player, used, enemyIndice, itemIndice, stairs;

} Map;

typedef struct{

	int nivel, inimigos, tamI, tamJ, indice;
	Map mapa[100][100]; 
} Nivel;

int main (){

	int i, j;
	Nivel niveis;
	FILE *arq;

	arq = fopen("database2.bin", "r");

	if(arq == NULL)
		return 0;


	while(fread(&niveis, sizeof(Nivel), 1, arq)){


		printf("\n\nMapa de nivel: %d\nNumero de inimigos no mapa: %d\nIndice: %d\n\n", niveis.nivel, niveis.inimigos, niveis.indice);

		for(i = 0; i < niveis.tamI; i++){
			for(j = 0; j < niveis.tamJ; j++){
				if(niveis.mapa[i][j].player)
					printf("P ");

				else if(niveis.mapa[i][j].wall)
					printf("X ");

				else if(niveis.mapa[i][j].stairs < 0)
					printf("< ");

				else if(niveis.mapa[i][j].stairs > 0)
					printf("> ");

				else if(niveis.mapa[i][j].enemyIndice >= 0)
					printf("E ");

				else if(niveis.mapa[i][j].itemIndice >= 0)
					printf("I ");


				else
					printf("  ");
			}

			printf("\n");
		}
	}
	
	fclose(arq);

	return 0;

}