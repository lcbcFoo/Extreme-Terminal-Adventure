/* Author: ETA Team *
 * Last Modification: 07/22/2015 by Chams */

#include <stdio.h>
#include <stdlib.h>


/* Define o struct do mapa */
typedef struct {

	int wall, player, used, enemyIndice, itemIndice,stairs;

} Map;

typedef struct{

	int nivel, inimigos, tamI, tamJ, deletado;
	Map mapa[100][100]; 
} Nivel;


int main (){

	int i, j, count, recebe, n, m, mapas, aux;
	int cont1,indice, flag = 0, subst = 0, del = 0;
	char comand;
	Nivel *nivel;	
	Nivel *vetMapas, auxMapa;
	FILE *arq;

	printf("Deseja:\n\n1- Criar mapas\n2- Substituir mapas\n3- Deletar mapas\n");
	printf("PS: NAO EXISTE MAPA DE NIVEL 0. PLX\n");
	do{
		scanf("%c", &comand);
	}while(!(comand >= '1' && comand <= '3'));

	if(comand == '1'){

		printf("Quantos mapas vc criara?\n");
		scanf("%d", &mapas);
		nivel = malloc(mapas * sizeof(Nivel));
	}

	else if(comand == '2'){

		mapas = 1;
		nivel = malloc(sizeof(Nivel));
		printf("Qual mapa deseja substituir?\n");
		scanf("%d", &subst);
	}

	/*================================================================================================= 
	O NIVEL DOS MAPAS DEVE COMEÇAR EM 1 PARA ESSA PARTE FUNCIONAR. NAO TENTE DELETAR UM MAPA DE NIVEL 0.
	ESTOU COLOCANDO ISSO AQUI PORQUE SEI QUE VAI CAUSAR PROBLEMAS NO FUTURO 
	=================================================================================================*/
	else{
		
		printf("Qual mapa deseja deletar?\n");
		scanf("%d", &del);
	}

	if(comand != '3'){

		for(count = 0; count < mapas; count++){

			nivel[count].deletado = 0;

			printf("Insira o tamanho do mapa m x n:\n");
			scanf("%d %d", &m, &n);

			if(!subst){
				printf("Insira o nivel do mapa:\n");
				scanf("%d", &recebe);
			}
			else
				recebe = subst;

			nivel[count].nivel = recebe;;
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

		if(subst){

			arq = fopen("database2.bin", "r+b");

			if(arq == NULL){
				printf("Erro ao abrir database!\n");
				return 0;
			}

			while(fread(&auxMapa, sizeof(Nivel), 1, arq) && auxMapa.nivel < subst);

			if(auxMapa.nivel == subst){

				fseek(arq, -1 * sizeof(Nivel), SEEK_CUR);

				fwrite(nivel, sizeof(Nivel), 1, arq);

				fclose(arq);
				free(nivel);

				printf("Substituicao feita com sucesso!\n");

				return 0;
			}

			else{

				printf("Mapa nao encontrado!\n");

				return 0;
			}
			
		}
	

		printf("Deseja zerar o arquivo de mapas? (y/n)\n");
		do{
			scanf("%c", &comand);
		}while(comand != 'y' && comand != 'n');

		if(comand == 'n')
			arq = fopen("database2.bin", "r+b");

		else
			arq = fopen("database2.bin", "w+b");

		if(arq == NULL){
			printf("Erro ao abrir database!\n");
			return 0;
		}


		fseek(arq, 0, SEEK_END);
		fwrite(nivel, mapas * sizeof(Nivel), 1, arq);
		free(nivel);
	
		/* Organiza os mapas de acordo com o nivel.
		 Sim, eu copio para um vetor auxiliar, nao vou organizar de dentro do arquivo hoje haha */


		rewind(arq);
		cont1=0;

		while(fread(&auxMapa, sizeof(Nivel), 1, arq)){
			cont1++;
		};

		vetMapas = malloc(cont1 * sizeof(Nivel));
		rewind(arq);
		i=0;

		while(fread(&vetMapas[i], sizeof(Nivel), 1, arq)){
			i++;
		};
		
		for(i = 0; i < cont1 - 1; i++){

			auxMapa = vetMapas[i];
			flag = 0;

			for(j = i+1; j < cont1; j++)

				if(vetMapas[j].nivel < auxMapa.nivel){
					auxMapa = vetMapas[j];
					indice = j;
					flag = 1;
				}
			
			if(flag){
				vetMapas[indice] = vetMapas[i];
				vetMapas[i] = auxMapa;
			}

		}

		rewind(arq);
		fwrite(vetMapas, cont1 * sizeof(Nivel), 1, arq);

		fclose(arq);	

		printf("\nSucesso moleque!!\n\n");

		return 0;

	}

	arq = fopen("database2.bin", "r+b");

	while(fread(&auxMapa, sizeof(Nivel), 1, arq) && auxMapa.nivel < del);

	if(auxMapa.deletado){

		printf("Mapa ja deletado. Deseja recupera-lo?(y/n)\n");
		do{
			scanf("%c", &comand);
		}while(comand != 'y' && comand != 'n');

		if(comand == 'y'){

			auxMapa.deletado = 0;

			fseek(arq, -1 * sizeof(Nivel), SEEK_CUR);
			fwrite(&auxMapa, sizeof(Nivel), 1, arq);
			fclose(arq);

			printf("\nSucesso moleque!!\n\n");

			return 0;
		}

		else
			printf("Ok.\n");

	}

	else if(auxMapa.nivel == del){

		fseek(arq, -1 * sizeof(Nivel), SEEK_CUR);

		auxMapa.deletado = 1;
		fwrite(&auxMapa, sizeof(Nivel), 1, arq);

		fclose(arq);

		printf("\nSucesso moleque!!\n\n");

		return 0;
	}


	else
		printf("Mapa nao encontrado :'(\n");

	return 0;
}