/* Author: ETA Team */

/* Tipos:
 * 1 - weapon
 * 2 - armor
 * 3 - pot */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "init.h"


int main (){

    int i, j, flag;
    int existentItems = 0;
    int newItems;
    char recebe;
    FILE *database;
    Item *items;
    Item read;

    database = fopen("items.bin", "r+b");


    if(database == NULL){
        printf("macumba\n");
        return 0;
    }

    while(fread(&read, sizeof(Item), 1, database))
        existentItems++;

    printf("Quantos itens serao criados?\n");
    scanf("%d", &newItems);

    items = malloc((existentItems + newItems) * sizeof(Item));

    rewind(database);

    fread(items, existentItems * sizeof(Item), 1, database);

    for(i = existentItems; i < existentItems + newItems; i++){

        flag = 1;
        printf("Nome: \n");
        scanf(" ");
        fgets(items[i].nome, 51, stdin);

        for(j = 0; items[i].nome[j]; j++)
            if(items[i].nome[j] == '\n')
                items[i].nome[j] = '\0';

        for(j = 0; (j < i) && (flag); j++)
            if(strcmp(items[i].nome, items[j].nome) == 0)
                flag = 0;

        if(flag){
            printf("Tipo: \n");
            scanf("%d", &items[i].tipo);

            printf("Valor: \n");
            scanf("%d", &items[i].valor);

            items[i].indice = i;
        }

        else{
            printf("Item ja contido na database, insira outro\n");
            i--;
        }
    }

    printf("\n\n\n");

    for(i = existentItems; i < existentItems + newItems; i++){
        printf("%s, tipo = %d, valor = %d, indice = %d\n", items[i].nome, items[i].tipo, items[i].valor, items[i].indice);
    }

    printf("Deseja continuar? (y/n)\n");
    scanf(" %c", &recebe);

    if(recebe != 'y')
        return 0;

    rewind(database);
    int total = existentItems + newItems;
    fwrite(&total, sizeof(int), 1, database);
    fseek(database, 0, SEEK_END);

    for(i = existentItems; i < total; i++)
        if(!fwrite(&items[i], sizeof(Item), 1, database)){
            printf("erro no item %d!\n", i + 1);
            return 0;
        }

    printf("Sucesso! Mais itens foram adicionados!\n");

    fclose(database);

    return 0;
}
