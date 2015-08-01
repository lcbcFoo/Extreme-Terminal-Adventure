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

int main (){

	int n = 0, recebe, aux, i, j, aux2;
	Enemy *enemies, auxE;
	FILE *arq;

	arq = fopen("inimigos.bin", "r+b");

	if(arq == NULL){
		printf("Erro ao abrir database de inimigos\n");
		return 0;
	}

	while(fread(&auxE, sizeof(Enemy), 1, arq))
		n++;

	printf("Voce deseja:\n1) Criar novos inimigos;\n2) Substituir inimigos ja criados;\n");

	do{
		scanf("%d", &recebe);
	}while((recebe != 1) && (recebe != 2));

	if(recebe == 1){
		printf("Quantos inimigos voce deseja criar?\n");
		scanf("%d", &aux2);
		
		enemies = malloc((n + aux2) * sizeof(Enemy));

		rewind(arq);

		fread(enemies, n * sizeof(Enemy), 1, arq);

		for(i = n; i < n + aux2; i++){

			for(j = 0; j < QUANT_ITENS; j++)
				enemies[i].dropItems[j] = 0;
			
			printf("Nome:\n");
			scanf(" ");
			fgets(enemies[i].nome, 51, stdin);

			for(j = 0; enemies[i].nome[j]; j++)
				if(enemies[i].nome[j] == '\n')
					enemies[i].nome[j] = '\0';

			printf("HP:\n");
			scanf("%d", &enemies[i].hp);
			printf("Ataque:\n");
			scanf("%d", &enemies[i].attack);
			printf("Defesa:\n");
			scanf("%d", &enemies[i].defense);
			printf("XP dada:\n");
			scanf("%d", &enemies[i].givenXP);
			printf("Drop Rate (de 0 a 1):\n");
			scanf("%lf", &enemies[i].dropRate);
			printf("Numero de itens dropaveis:\n");
			scanf("%d", &aux);
			printf("Insira os %d indices dos itens dropaveis\n", aux);

			for(j = 0; j < aux; j++){
				scanf("%d", &recebe);
				enemies[i].dropItems[recebe] = 1;
			}
		}

		fwrite(&enemies[n], aux2 * sizeof(Enemy), 1, arq);
		fclose(arq);
		free(enemies);

		printf("Sucesso!\n");
	}

	else{
		enemies = malloc(n * sizeof(Enemy));
		rewind(arq);
		fread(enemies, n * sizeof(Enemy), 1, arq);

		for(i = 0; i < n; i++){
			printf("\nInimigo de indice %d\nNome: %s\nHP: %d\nAttack: %d\nDefesa: %d\n", i, enemies[i].nome, enemies[i].hp, enemies[i].attack, enemies[i].defense);
			printf("XP: %d\nDrop Rate: %f\nItens dropaveis:\n", enemies[i].givenXP, enemies[i].dropRate);

			for(j = 0; j < QUANT_ITENS; j++)
				if(enemies[i].dropItems[j])
					printf("%d ", j);

			printf("\n\n");	
		}

		printf("Qual o indice do inimigo que voce deseja modificar?\n");
		scanf("%d", &i);

		printf("Insira os stats do novo inimigo:\n\n");

		for(j = 0; j < QUANT_ITENS; j++)
			enemies[i].dropItems[j] = 0;
			
		printf("Nome:\n");
		scanf(" ");
		fgets(enemies[i].nome, 51, stdin);

		for(j = 0; enemies[i].nome[j]; j++)
			if(enemies[i].nome[j] == '\n')
				enemies[i].nome[j] = '\0';

		printf("HP:\n");
		scanf("%d", &enemies[i].hp);
		printf("Ataque:\n");
		scanf("%d", &enemies[i].attack);
		printf("Defesa:\n");
		scanf("%d", &enemies[i].defense);
		printf("XP dada:\n");
		scanf("%d", &enemies[i].givenXP);
		printf("Drop Rate (de 0 a 1):\n");
		scanf("%lf", &enemies[i].dropRate);
		printf("Numero de itens dropaveis:\n");
		scanf("%d", &aux);
		printf("Insira os %d indices dos itens dropaveis\n", aux);

		for(j = 0; j < aux; j++){
			scanf("%d", &recebe);
			enemies[i].dropItems[recebe] = 1;
		}

		rewind(arq);
		fwrite(enemies, n * sizeof(Enemy), 1, arq);
		fclose(arq);
		free(enemies);

		printf("Sucesso!\n");
	}

	return 0;
}
