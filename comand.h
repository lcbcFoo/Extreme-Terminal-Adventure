/* Author: ETA Team *
 * Last Modification: 07/16/2015 by Foo */


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

/* Define quantidade de itens e tamanho da bag */
#define TAM_BAG 5
#define QUANT_ITENS 2


/* Mostra os comandos permitidos para movimentar o personagem */
void comandList(){

	printf("\n\nComandos disponiveis durante o jogo:\n");
	printf("w - anda para cima/ataca inimigo que esteja na posicao de cima no mapa\n");
	printf("d - anda para direita/ataca inimigo que esteja na posicao da direita no mapa\n");
	printf("s - anda para baixo/ataca inimigo que esteja na posicao de baixo no mapa\n");
	printf("a - anda para esquerda/ataca inimigo que esteja na posicao da esquerda do mapa\n");
	printf("b - abre o inventario\n");
	printf("- - acessa o menu do jogo\n\n\n");
}

/* Salva o jogo */
int saveGame(Map map[TAM][TAM], Player *player, Enemy *enemies, Bag * bag){

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

		for(i = 0; i < TAM_BAG; i++)
			if(!fwrite(&bag[i], sizeof(Bag), 1, arq)){
				fclose(arq);
				return 0;
			}	

		fclose(arq);

		return 1;
	}

	return 0;
}

/* Menu de comandos de controle sobre o jogo */
int menu(Map map[TAM][TAM], Player *player, Enemy *enemies, Bag *bag){

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
			if(saveGame(map, player, enemies, bag)){
				system("clear");
				printf("\n\nJogo salvo!\n\n");
			}	

			else
				printf("O jogo nao pode ser salvo, tente novamente\n\n");

			return menu(map, player, enemies, bag);
		}

		else if(recebe == 'r'){
			system("clear");
			return 1;
		}	

		else if(recebe == 'q'){
			system("clear");
			return 0;
		}

		else if(recebe == 'l'){
			system("clear");
			comandList();
			return menu(map, player, enemies, bag);
		}

	}while(flag);

	return 1;
}

/* Funcao que usa pocoes */
void usaPot(Bag *bag, Player *player){

	(*player).hp += (*bag).item.valor;

	if((*player).hp > (*player).MaxHP)
		(*player).hp = (*player).MaxHP;

	(*bag).quantidade--;

	if((*bag).quantidade == 0)
		(*bag).used = 0;
}

/* Funcao que printa o que tem dentro da mochila e permite que itens sejam usados */
void printBag(Bag *bag, Player *player){

	int i, flag = 1;
	char recebe;

	system("clear");

	printf("Bag:\n\n");

	for(i = 0; i < TAM_BAG; i++){
		if(bag[i].used){
			flag = 0;
			printf("%d - %s (%d)\n", i + 1, bag[i].item.nome, bag[i].quantidade);

			if(bag[i].item.tipo == 1)
				printf("    Dano: %d\n\n", bag[i].item.valor);

			else if(bag[i].item.tipo == 2)
				printf("    Defesa: %d\n\n", bag[i].item.valor);

			else if(bag[i].item.tipo == 3)
				printf("    Recupera %d vida\n\n", bag[i].item.valor);
		}
	}

	if(flag)
		printf("Inventario vazio!!\n\n");

	printf("Comandos possiveis: \nDigite o indice do item que deseja usar\nr - retorna ao mapa\n\n");

	flag = 1;

	do{
		scanf(" %c", &recebe);

		if(recebe == 'r')
			return;

		else if((recebe - '1' >= 0) && (recebe - '1' < TAM_BAG) && (bag[recebe - '1'].used)){
			flag = 0;

			if(bag[recebe - '1'].item.tipo == 3)
				usaPot(&bag[recebe - '1'], player);
		}

	}while(flag);
}

/* Funcao que pega o item de uma posicao do mapa */
void pegaItem(Player *player, Item item, Map *position, Bag *bag){

	int i, flag = 1;

	/* Verifica se voce ja tem aquele item */
	for(i = 0; (i < TAM_BAG) && (flag); i++){
		if((bag[i].used) && (strcmp(bag[i].item.nome, item.nome) == 0)){
			flag = 0;
			bag[i].quantidade++;
			bag[i].used = 1;
			(*position).itemIndice = -1;
			(*position).used = 0;
		}
	}

	/* Verifica se existe uma posicao vazia na bag */
	for(i = 0; (i < TAM_BAG) && (flag); i++){
		if(bag[i].used == 0){
			bag[i].item = item;
			bag[i].quantidade = 1;
			bag[i].used = 1;
			flag = 0;
			(*position).used = 0;
			(*position).itemIndice = -1;
		}
	}

	/* Imprime se vc pegou o item ou se sua bag tava cheia */
	if(flag){
		printf("Seu inventario esta cheio!!\n\n");
	}

	else
		printf("Voce encontrou um item: %s\n\n", item.nome);
}

/* Executa comandos de controle de personagem ou acessa o menu */
int executeComand(char comand, Player *player, Map map[TAM][TAM], Enemy *enemies, Bag *bag, Item *itens){

	/* Recebe os comandos durante o jogo e os executa */
	/* Abre o menu */
	if(comand == '-')
		return menu(map, player, enemies, bag);

	else if(comand == 'b')
		printBag(bag, player);

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

		/* Caso em que ha um item na posicao */
		else if(map[(*player).y][(*player).x - 1].itemIndice != -1)
			pegaItem(player, itens[map[(*player).y][(*player).x - 1].itemIndice], 
					&map[(*player).y][(*player).x - 1], bag);

		/* Movimenta os inimigos e retorna se o jogador ainda esta vivo */
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

		/* Caso em que ha um item na posicao */
		else if(map[(*player).y][(*player).x + 1].itemIndice != -1)
			pegaItem(player, itens[map[(*player).y][(*player).x + 1].itemIndice], 
					&map[(*player).y][(*player).x + 1], bag);
	
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

		/* Caso em que ha um item na posicao */
		else if(map[(*player).y - 1][(*player).x].itemIndice != -1)
			pegaItem(player, itens[map[(*player).y - 1][(*player).x].itemIndice], 
					&map[(*player).y - 1][(*player).x], bag);

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

		/* Caso em que ha um item na posicao */
		else if(map[(*player).y + 1][(*player).x].itemIndice != -1)
			pegaItem(player, itens[map[(*player).y + 1][(*player).x].itemIndice], 
					&map[(*player).y + 1][(*player).x], bag);
	
		return enemyAction(player, map, enemies);
	}

	return 1;
}