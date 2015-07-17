/* Author: ETA Team *
 * Last Modification: 07/16/2015 by Foo */


#include <stdio.h>
#include <stdlib.h>

#define QTS_TINHA_ANTES 2
#define NUM_ITENS_NOVO 2

typedef struct {
    /* Tipos:
     * 1 - arma
     * 2 - armadura
     * 3 - pot */
    int indice, valor, tipo;
    char nome[51];
} Item;

int main (){

	int i, j;
	char recebe;
	FILE *database;
	Item novos[NUM_ITENS_NOVO];

	for(i = QTS_TINHA_ANTES; i < NUM_ITENS_NOVO; i++){
		printf("Nome: \n");
		scanf(" ");
		fgets(novos[i].nome, 51, stdin);

		printf("Tipo: \n");
		scanf("%d", &novos[i].tipo);

		printf("Valor: \n");
		scanf("%d", &novos[i].valor);

		novos[i].indice = i;

		for(j = 0; novos[i].nome[j]; j++)
			if(novos[i].nome[j] == '\n')
				novos[i].nome[j] = '\0';
	}

	printf("\n\n\n");

	for(i = QTS_TINHA_ANTES; i < NUM_ITENS_NOVO; i++){
		printf("%s, tipo = %d, valor = %d, indice = %d\n", novos[i].nome, novos[i].tipo, novos[i].valor, novos[i].indice);
	}

	printf("Deseja continuar? (y/n)\n");
	scanf(" %c", &recebe);

	if(recebe != 'y')
		return 0;

	database = fopen("database.bin", "r+b");

	if(database == NULL)
		return 0;

	fseek(database, 0, SEEK_END);

	for(i = 0; i < NUM_ITENS_NOVO; i++)
		if(!fwrite(&novos[i], sizeof(Item), 1, database)){
			printf("erro no item %d!\n", i + 1);
			return 0;
		}

	printf("sucesso! LEMBRE DE ATUALIZAR O DEFINE COM O NOVO NUMERO DE ITENS!!!!\n");

	fclose(database);

	return 0;
}