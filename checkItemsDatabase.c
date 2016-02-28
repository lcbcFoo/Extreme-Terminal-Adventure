/* Author: ETA Team *
 * Last Modification: 07/16/2015 by Foo */



#include <stdio.h>
#include "init.h"


int main(){

	int count = 0;
	Item item;
	FILE *arq;

	arq = fopen("items.bin", "rb");

	while(fread(&item, sizeof(Item), 1, arq)){
		count++;
		printf("Nome: %s\nValor: %d\nTipo: %d\nIndice: %d\n\n", item.nome, item.valor, 
																item.tipo, count - 1);
	}

	fclose(arq);

	return 0;
}