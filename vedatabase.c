/* Author: ETA Team *
 * Last Modification: 07/16/2015 by Foo */



#include <stdio.h>

/* Struct de itens */
typedef struct {
    /* Tipos:
     * 1 - arma
     * 2 - armadura
     * 3 - pot */
    int indice, valor, tipo;
    char nome[51];
} Item;

int main(){

	int count = 0;
	Item item;
	FILE *arq;

	arq = fopen("database.bin", "rb");

	while(fread(&item, sizeof(Item), 1, arq)){
		count++;
		printf("Nome: %s\nValor: %d\nTipo: %d\nIndice: %d\n\n", item.nome, item.valor, 
																item.tipo, count - 1);
	}

	fclose(arq);

	return 0;
}