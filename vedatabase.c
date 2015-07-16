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
		printf("nome: %s\nvalor: %d\ntipo: %d\n", item.nome, item.valor, item.tipo);
	}

	fclose(arq);

	return 0;
}