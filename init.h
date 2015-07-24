/* Author: ETA Team *
 * Last Modification: 07/22/2015 by Foo */



/* Biblioteca que inicializa o jogo ou carrega a partida salva */


/* Define stats do jogador */
#define BASE_HP 20
#define BASE_ATTACK 3
#define BASE_DEF 1
#define BASE_NEXT_LEVEL 10

/* Define stats de cada inimigo */
#define ENEMY_1_HP 10
#define ENEMY_1_BASE_ATTACK 6
#define ENEMY_1_BASE_DEF 3
#define ENEMY_1_XP 11

/* Define quantidade de itens e tamanho da bag */
#define TAM_BAG 5
#define QUANT_ITENS 5

/* Inicializa o jogador */
void playerInit(Player *player, Item *itens, Nivel *nivel){

	int aux = -1;

	/* Inicializa os stas do jogador com os stats base */
	(*player).hp = BASE_HP;
	(*player).level = 1;
	(*player).XP = 0;
	(*player).NextLevel = BASE_NEXT_LEVEL;
	(*player).MaxHP = BASE_HP;
	(*player).weapon = itens[2];
	(*player).attack = BASE_ATTACK + (*player).weapon.valor;
	(*player).gear = itens[3];
	(*player).defense = BASE_DEF + (*player).gear.valor;
	(*player).nivelAtual = 0;

	srand(time(NULL));

	do{
		/* Cria a posicao que o jogador sera colocado no mapa */
		while((aux < 1) || (aux >= (*nivel).tamI - 1))
			aux = rand() % (*nivel).tamI;	

		(*player).y = aux;
		aux = -1;

		while((aux < 1) || (aux >= (*nivel).tamJ - 1))
			aux = rand() % (*nivel).tamJ;

		(*player).x = aux;

	}while((*nivel).mapa[(*player).y][(*player).x].used);

	(*nivel).mapa[(*player).y][(*player).x].used = 1;
	(*nivel).mapa[(*player).y][(*player).x].player = 1;
}

void bagInit(Bag *bag){

	int i;

	for(i = 0; i < TAM_BAG; i++)
		bag[i].used = 0;

}

void enemyInit(Enemy *enemy){

	/* Inicializa os inimigos com seus stats base */

	(*enemy).hp = ENEMY_1_HP;
	(*enemy).defense = ENEMY_1_BASE_DEF;
	(*enemy).attack = ENEMY_1_BASE_ATTACK;
	(*enemy).givenXP = ENEMY_1_XP;
}

void enemyPositions(Nivel nivel, Enemy *enemies){

	int i, j;

	for(i = 0; i < nivel.tamI; i++)
		for(j = 0; j < nivel.tamJ; j++)
			if(nivel.mapa[i][j].enemyIndice >= 0){
				enemies[nivel.mapa[i][j].enemyIndice].y = i;
				enemies[nivel.mapa[i][j].enemyIndice].x = j;
			}
}

/* Verfica se existe e carrega partida salva */

/* NAO ESTA FUNCIONANDO, NAO TENTE CARREGAR UM JOGO SALVO */
int gameLoad(Player *player, Nivel *niveis, Enemy **enemies, Bag *bag, int n){

	int i;
	char read;
	FILE *arq;

	arq = fopen("data.bin", "rb");

	/* Caso o arquivo de salvar o jogo exista, permite que o jogo seja carregado *
	 * caso contrario retorna 0 e cria um novo mapa */
	if(arq != NULL){
		printf("Voce deseja continuar o jogo salvo? (y/n)\n");
		scanf(" %c", &read);

		if(read == 'y'){
			fread(niveis, n * sizeof(Nivel), 1, arq);
			fread(player, sizeof(Player), 1, arq);

			for(i = 0; i < n; i++)
				fread(&enemies[i], niveis[i].inimigos * sizeof(Enemy), 1, arq);
			
			fread(bag, sizeof(Bag), TAM_BAG, arq);

			fclose(arq);
			remove("data.bin");
			system("clear");
			return 1;
		}

		system("clear");

		fclose(arq);	
	}

	return 0;
}
