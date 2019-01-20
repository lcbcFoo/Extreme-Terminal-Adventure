/* Author: ETA Team */



#include <stdio.h>
#include "init.h"


int main(){
    Item item;
    FILE *arq;
    int existentItems;
    int count = 0;

    arq = fopen("items.bin", "rb");

    if(arq == NULL){
        printf("Erro\n");
        return 0;
    }

    fread(&existentItems, sizeof(int), 1, arq);

    while(fread(&item, sizeof(Item), 1, arq)){
        printf("Nome: %s\nValor: %d\nTipo: %d\nIndice: %d\n\n", item.nome, item.valor,
                item.tipo, count);
        count++;
    }
    fclose(arq);

    return 0;
}
