/* Author: ETA Team *
 * Last Modification: 07/17/2015 by Foo */

/* Pot menor, pot maior, espada de madeira, trapo velho, espada cega */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QTS_TINHA_ANTES 5
#define NUM_ITENS_NOVO 5

typedef struct {
    /* Tipos:
     * 1 - arma
     * 2 - armadura
     * 3 - pot */
    int indice, valor, tipo;
    char nome[51];
} Item;

int main (){

	int i, j, flag;
	char recebe;
	FILE *database;
	Item novos[NUM_ITENS_NOVO];

	database = fopen("database.bin", "r+b");


	if(database == NULL){
		printf("macumba\n");
		return 0;
	}

	fread(novos, QTS_TINHA_ANTES * sizeof(Item), 1, database);

	for(i = QTS_TINHA_ANTES; i < NUM_ITENS_NOVO; i++){
		flag = 1;
		printf("Nome: \n");
		scanf(" ");
		fgets(novos[i].nome, 51, stdin);

		for(j = 0; novos[i].nome[j]; j++)
			if(novos[i].nome[j] == '\n')
				novos[i].nome[j] = '\0';

		for(j = 0; (j < i) && (flag); j++)
			if(strcmp(novos[i].nome, novos[j].nome) == 0)
				flag = 0;

		if(flag){
			printf("Tipo: \n");
			scanf("%d", &novos[i].tipo);

			printf("Valor: \n");
			scanf("%d", &novos[i].valor);

			novos[i].indice = i;
		}	

		else{
			printf("Item ja contido na database, insira outro\n");
			i--;
		}	
	}

	printf("\n\n\n");

	for(i = QTS_TINHA_ANTES; i < NUM_ITENS_NOVO; i++){
		printf("%s, tipo = %d, valor = %d, indice = %d\n", novos[i].nome, novos[i].tipo, novos[i].valor, novos[i].indice);
	}

	printf("Deseja continuar? (y/n)\n");
	scanf(" %c", &recebe);

	if(recebe != 'y')
		return 0;

	fseek(database, 0, SEEK_END);

	for(i = QTS_TINHA_ANTES; i < NUM_ITENS_NOVO; i++)
		if(!fwrite(&novos[i], sizeof(Item), 1, database)){
			printf("erro no item %d!\n", i + 1);
			return 0;
		}

	printf("sucesso! LEMBRE DE ATUALIZAR O DEFINE COM O NOVO NUMERO DE ITENS!!!!\n");

	fclose(database);

	return 0;
}