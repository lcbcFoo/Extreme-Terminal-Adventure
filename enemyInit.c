/* Author: ETA Team *
 * Last Modification: 08/01/2015 by Foo*/


#include <stdio.h>
#include <stdlib.h>

#define QUANT_ITENS 6


/* Define o struct de inimigos */
typedef struct {

	int hp, attack, defense, givenXP, x, y, dropItems[QUANT_ITENS];
	double dropRate;
	char nome[51];

} Enemy;

typedef struct {

	int wall, player, used, enemyIndice, itemIndice,stairs;

} Map;

typedef struct{

	int nivel, inimigos, tamI, tamJ, indice;
	Map mapa[30][30]; 
} Nivel;


int main (){

	int n = 0, i, j, k, count = 0, recebe, aux;
	FILE *arq;
	Nivel aux2, *niveis;
	Enemy **enemies, *padrao, lixo;

	arq = fopen("database2.bin", "rb");

	if(arq == NULL){
		printf("Erro ao abrir database de mapas\n");
		return 0;
	}

	while(fread(&aux2, sizeof(Nivel), 1, arq))
		n++;

	rewind(arq);
	niveis = malloc(n * sizeof(Nivel));
	enemies = malloc(n * sizeof(Enemy*));

	fread(niveis, sizeof(Nivel), n, arq);
	fclose(arq);

	for(i = 0; i < n; i++)
		enemies[i] = malloc(niveis[i].inimigos * sizeof(Enemy));

	arq = fopen("inimigos.bin", "rb");

	if(arq == NULL){
		printf("Erro ao abrir database de inimigos prontos!\n");
		return 0;
	}

	aux = 0;

	while(fread(&lixo, sizeof(Enemy), 1, arq))
		aux++;

	rewind(arq);
	padrao = malloc(aux * sizeof(Enemy));

	fread(padrao, aux * sizeof(Enemy), 1, arq);

	fclose(arq);

	arq = fopen("database3.bin", "wb");

	if(arq == NULL){
		printf("Erro ao limpar database!\n");
		return 0;
	}

	for(k = 0; k < n; k++){
		count = 0;
		printf("Os numeros no mapa a seguir representam as posicoes que possuem inimigos na ordem em que voce deve inicializa-los\n\n");

		for(i = 0; i < niveis[k].tamI; i++){
			for(j = 0; j < niveis[k].tamJ; j++){

				if(niveis[k].mapa[i][j].player)
					printf("P ");

				else if(niveis[k].mapa[i][j].wall)
					printf("X ");

				else if(niveis[k].mapa[i][j].stairs < 0)
					printf("< ");

				else if(niveis[k].mapa[i][j].stairs > 0)
					printf("> ");

				else if(niveis[k].mapa[i][j].enemyIndice >= 0){
					printf("%d ", 1 + count);
					enemies[k][count].y = i;
					enemies[k][count++].x = j;
				}

				else if(niveis[k].mapa[i][j].itemIndice >= 0)
					printf("I ");

				else
					printf("  ");
			}

			printf("\n");
		}

		printf("\n\nInimigos disponiveis:\n\n");

		for(i = 0; i < aux; i++){
			printf("\nInimigo de indice %d\nNome: %s\nHP: %d\nAttack: %d\nDefesa: %d\n", i, padrao[i].nome, padrao[i].hp, padrao[i].attack, padrao[i].defense);
			printf("XP: %d\nDrop Rate: %f\nItens dropaveis:\n", padrao[i].givenXP, padrao[i].dropRate);

			for(j = 0; j < QUANT_ITENS; j++)
				if(padrao[i].dropItems[j])
					printf("%d ", j);

			printf("\n\n");	
		}	

		
		for(i = 0; i < niveis[k].inimigos; i++){
			printf("\n\nDigite qual inimigo sera colocado na posicao %d:\n", i + 1);
			scanf("%d", &recebe);

			enemies[k][i] = padrao[recebe];
		}	
	}

	for (i = 0; i < n; i++){
   		fwrite(enemies[i], sizeof(**enemies), niveis[i].inimigos, arq);
		free(enemies[i]);
	}

	free(enemies);
   	fclose(arq);

	return 0;
}