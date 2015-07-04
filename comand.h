/* Author: Lucas de Camargo Barros de Castro *
 * Last Modification: 07/03/2015 by Lucas Castro */

/* Biblioteca que executa comandos do jogo */


/* Define stats do jogador */
#define BASE_HP 20
#define BASE_ATTACK 5
#define BASE_DEF 3
#define BASE_NEXT_LEVEL 10

/* Define stats de cada inimigo */
#define ENEMY_1_HP 10
#define ENEMY_1_BASE_ATTACK 6
#define ENEMY_1_BASE_DEF 3
#define ENEMY_1_XP 11

/* Define tamanho do mapa e quantidade de inimigos */
#define TAM 10
#define NUM_INIMIGOS 4


/* Mostra os comandos permitidos para movimentar o personagem */
void comandList(){

	printf("\n\nComandos disponiveis durante o jogo:\n");
	printf("w - anda para cima/ataca inimigo que esteja na posicao de cima no mapa\n");
	printf("d - anda para direita/ataca inimigo que esteja na posicao da direita no mapa\n");
	printf("s - anda para baixo/ataca inimigo que esteja na posicao de baixo no mapa\n");
	printf("a - anda para esquerda/ataca inimigo que esteja na posicao da esquerda do mapa\n");
	printf("- - acessa o menu do jogo\n\n\n");
}

/* Salva o jogo */
int saveGame(Map map[TAM][TAM], Player *player, Enemy *enemies){

	int i, j;
	FILE *arq;

	arq = fopen("data.bin", "wb");

	/* Salva os elementos contidos no mapa, os stats do jogador e os inimigos *
	 * existentes no arquivo que salva o jogo */
	if(arq != NULL){

		for(i = 0; i < TAM; i++)
			for(j = 0; j < TAM; j++)
				if(!fwrite(&map[i][j], sizeof(Map), 1, arq)){
					fclose(arq);
					return 0;
				}	

		if(!fwrite(player, sizeof(Player), 1, arq)){
			fclose(arq);
			return 0;
		}

		for(i = 0; i < NUM_INIMIGOS; i++)
			if(!fwrite(&enemies[i], sizeof(Enemy), 1, arq)){
				fclose(arq);
				return 0;
			}


		fclose(arq);

		return 1;
	}

	return 0;
}

/* Menu de comandos de controle sobre o jogo */
int menu(Map map[TAM][TAM], Player *player, Enemy *enemies){

	int flag = 1;
	char recebe;

	/* Lista os comandos disponiveis e salva executa o comando desejado pelo jogador */
	printf("\n\n==== Menu ====\n\nOpcoes de comando:\n");
	printf("s - salvar o jogo\n");
	printf("r - retornar ao jogo\n");
	printf("q - deixar o jogo (tenha certeza que voce salvou o jogo antes!)\n");
	printf("l - lista de comandos de jogo\n");

	do{
		scanf(" %c", &recebe);

		if(recebe == 's'){
			if(saveGame(map, player, enemies))
				printf("\n\nJogo salvo!\n\n");

			else
				printf("O jogo nao pode ser salvo, tente novamente\n\n");

			return menu(map, player, enemies);
		}

		else if(recebe == 'r')
			return 1;

		else if(recebe == 'q')
			return 0;

		else if(recebe == 'l'){
			comandList();
			return menu(map, player, enemies);
		}

	}while(flag);

	return 1;
}

/* Executa comandos de controle de personagem ou acessa o menu */
int executeComand(char comand, Player *player, Map map[TAM][TAM], Enemy *enemies){

	/* Recebe os comandos durante o jogo e os executa */
	/* Abre o menu */
	if(comand == '-')
		return menu(map, player, enemies);

	/* Comandos de movimento / combate */
	else if(((*player).x > 0) && (comand == 'a')) {

		/* Esse eh o caso de movimento */
		if(map[(*player).y][(*player).x - 1].used == 0){
			map[(*player).y][(*player).x].player = 0;
			map[(*player).y][(*player).x].used = 0;

			(*player).x--;
			map[(*player).y][(*player).x].player = 1;
			map[(*player).y][(*player).x].used = 1;
		}
		
		/* Esse eh o caso de combate, chama a funcao combate */
		else if(map[(*player).y][(*player).x - 1].enemyIndice != -1)
			combate(player, &enemies[map[(*player).y][(*player).x - 1].enemyIndice], 
					&map[(*player).y][(*player).x - 1]);	

		return enemyAction(player, map, enemies);
	}

	/* Idem para os d, w, s */
	else if(((*player).x < TAM - 1) && (comand == 'd')){

		if(map[(*player).y][(*player).x + 1].used == 0){
			map[(*player).y][(*player).x].player = 0;
			map[(*player).y][(*player).x].used = 0;

			(*player).x++;
			map[(*player).y][(*player).x].player = 1;
			map[(*player).y][(*player).x].used = 1;
		}
		
		else if(map[(*player).y][(*player).x + 1].enemyIndice != -1)
			combate(player, &enemies[map[(*player).y][(*player).x + 1].enemyIndice], 
					&map[(*player).y][(*player).x + 1]);	
	
		return enemyAction(player, map, enemies);
	}

	else if(((*player).y > 0) && (comand == 'w')){

		if(map[(*player).y - 1][(*player).x].used == 0){
			map[(*player).y][(*player).x].player = 0;
			map[(*player).y][(*player).x].used = 0;

			(*player).y--;
			map[(*player).y][(*player).x].player = 1;
			map[(*player).y][(*player).x].used = 1;
		}
		
		else if(map[(*player).y - 1][(*player).x].enemyIndice != -1)
			combate(player, &enemies[map[(*player).y - 1][(*player).x].enemyIndice], 
					&map[(*player).y - 1][(*player).x]);

		return enemyAction(player, map, enemies);
	}
	
	else if(((*player).y < TAM - 1) && (comand == 's')){

		if(map[(*player).y + 1][(*player).x].used == 0){
			map[(*player).y][(*player).x].player = 0;
			map[(*player).y][(*player).x].used = 0;

			(*player).y++;
			map[(*player).y][(*player).x].player = 1;
			map[(*player).y][(*player).x].used = 1;
		}
		
		else if(map[(*player).y + 1][(*player).x].enemyIndice != -1)
			combate(player, &enemies[map[(*player).y + 1][(*player).x].enemyIndice], 
					&map[(*player).y + 1][(*player).x]);
	
		return enemyAction(player, map, enemies);
	}

	return 1;
}