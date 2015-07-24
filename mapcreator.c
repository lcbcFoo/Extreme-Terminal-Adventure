/* Author: ETA Team *
 * Last Modification: 07/23/2015 by Foo*/

#include <stdio.h>
#include <stdlib.h>


/* Define o struct do mapa */
typedef struct {

	int wall, player, used, enemyIndice, itemIndice,stairs;

} Map;

typedef struct{

	int nivel, inimigos, tamI, tamJ, indice;
	Map mapa[100][100]; 
} Nivel;

/* Recebe um mapa novo */
void RecebeMapa(Nivel *nivel, int mapas, int next){

	int i, j, count, aux, m, n, recebe;
	char comand;

	for(count = 0; count < mapas; count++){

		printf("Insira o tamanho do mapa m x n:\n");
		scanf("%d %d", &m, &n);

		printf("Insira o nivel do mapa:\n");
		scanf("%d", &recebe);

		nivel[count].nivel = recebe;
		nivel[count].indice = next++;
		nivel[count].tamI = m;
		nivel[count].tamJ = n;

		aux = 0;

		printf("\n\nPara cada posicao voce deve digitar 0, 1, 2 ou 3, onde:\n0 - posicao vazia;\n1 - muro;\n2 - inimigo;\n3 - item");
		printf("\n4 - escada que sobe;\n5- escada que desce\n");
		printf("(pressione enter, pois sera solicitado o indice do item na database!)\n\n\n");

		for(i = 0; i < m; i++){
			for(j = 0; j < n; j++){
				scanf("%d", &recebe);

				if(!recebe){
					nivel[count].mapa[i][j].used = 0;
					nivel[count].mapa[i][j].enemyIndice = -1;
					nivel[count].mapa[i][j].itemIndice = -1;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 0;
					nivel[count].mapa[i][j].stairs = 0;
				}

				else if(recebe == 1){
					nivel[count].mapa[i][j].used = 1;
					nivel[count].mapa[i][j].enemyIndice = -1;
					nivel[count].mapa[i][j].itemIndice = -1;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 1;
					nivel[count].mapa[i][j].stairs = 0;
				}

				else if(recebe == 2){
					nivel[count].mapa[i][j].used = 0;
					nivel[count].mapa[i][j].enemyIndice = nivel[count].nivel;
					nivel[count].mapa[i][j].itemIndice = -1;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 0;
					nivel[count].mapa[i][j].stairs = 0;

					aux++;
				}

				else if(recebe == 3){

					printf("Insira o indice do item de acordo com a database:\n");
					scanf("%d", &recebe);

					nivel[count].mapa[i][j].used = 1;
					nivel[count].mapa[i][j].enemyIndice = -1;
					nivel[count].mapa[i][j].itemIndice = recebe;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 0;
					nivel[count].mapa[i][j].stairs = 0;
				}

				else if(recebe == 4){

					nivel[count].mapa[i][j].used = 1;
					nivel[count].mapa[i][j].enemyIndice = 0;
					nivel[count].mapa[i][j].itemIndice = 0;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 0;
					nivel[count].mapa[i][j].stairs = -1;
				}

				else {

					nivel[count].mapa[i][j].used = 1;
					nivel[count].mapa[i][j].enemyIndice = 0;
					nivel[count].mapa[i][j].itemIndice = 0;
					nivel[count].mapa[i][j].player = 0;
					nivel[count].mapa[i][j].wall = 0;
					nivel[count].mapa[i][j].stairs = 1;
				}
			}

			nivel[count].inimigos = aux;
		}

		printf("Mapa inserido:\n\n");

		for(i = 0; i < m; i++){
			for(j = 0; j < n; j++){
				if(nivel[count].mapa[i][j].player)
					printf("P ");

				else if(nivel[count].mapa[i][j].wall)
					printf("X ");

				else if(nivel[count].mapa[i][j].stairs < 0)
					printf("< ");

				else if(nivel[count].mapa[i][j].stairs > 0)
					printf("> ");

				else if(nivel[count].mapa[i][j].enemyIndice >= 0)
					printf("E ");

				else if(nivel[count].mapa[i][j].itemIndice >= 0)
					printf("I ");


				else
					printf("  ");
			}

			printf("\n");
		}

		printf("\n\nDeseja manter esse mapa? (y/n)\n");
		scanf(" %c", &comand);

		if(comand != 'y'){
			count--;
			printf("Mapa descartado. Digite outro mapa:\n\n\n");
		}

		else
			printf("Mapa salvo!\n\n\n");
	}
}

int main (){

	int mapas, subst = 0, next = 0;
	char comand;
	Nivel *nivel, auxMapa;
	FILE *arq;

	printf("Deseja:\n\n1- Criar mapas\n2- Substituir mapas\n3 - Limpar database\n");
	printf("PS: NAO EXISTE MAPA DE NIVEL 0. PLX\n");
	do{
		scanf(" %c", &comand);
	}while(!(comand >= '1' && comand <= '3'));

	arq = fopen("database2.bin", "r+b");

	if(arq == NULL){
		printf("Merda ao abrir arquivo\n");
		return 0;
	}

	/* Conta quantos mapas tem na database para acertar o indice de mapas novos */
	while(fread(&auxMapa, sizeof(Nivel), 1, arq))
		next++;

	/* Cria novos mapas */
	if(comand == '1'){

		printf("Quantos mapas vc criara?\n");
		scanf("%d", &mapas);
		nivel = malloc(mapas * sizeof(Nivel));
		RecebeMapa(nivel, mapas, next);
		fseek(arq, 0, SEEK_END);

		fwrite(nivel, mapas * sizeof(Nivel), 1, arq);

	}

	/* Substitui um mapa */
	else if(comand == '2'){

		if(next == 0)
			printf("Nao ha nenhum mapa na database\n");

		else{
			mapas = 1;
			nivel = malloc(sizeof(Nivel));

			printf("Insira o indice do mapa que deseja substituir (Indices disponiveis: 0 - %d)\n", next - 1);
			scanf("%d", &subst);

			RecebeMapa(nivel, mapas, subst);

			fseek(arq, subst * sizeof(Nivel), SEEK_SET);

			fwrite(nivel, sizeof(Nivel), 1, arq);

		}

	}

	/*================================================================================================= 
	O NIVEL DOS MAPAS DEVE COMEÇAR EM 1 PARA ESSA PARTE FUNCIONAR. NAO TENTE DELETAR UM MAPA DE NIVEL 0.
	ESTOU COLOCANDO ISSO AQUI PORQUE SEI QUE VAI CAUSAR PROBLEMAS NO FUTURO 
	=================================================================================================*/

	/* Apaga a bagaca inteira */
	else if(comand == '3'){
		fclose(arq);

		printf("Certeza que quer apagar tudo? (y/n)\n");
		scanf(" %c", &comand);

		if(comand == 'y'){

			arq = fopen("database2.bin", "wb");

			if(arq == NULL){
				printf("Merda ao abrir a database\n");
				return 0;
			}

			printf("Database limpinha, espero que nao tenha sido burrada sua cara\n");
		}
	}

	return 0;
}