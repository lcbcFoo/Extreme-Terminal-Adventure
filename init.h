/* Author: ETA Team *
 * Last Modification: 07/03/2015 by Foo */



/* Biblioteca que inicializa o jogo ou carrega a partida salva */


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

/* Inicializa o jogador */
void playerInit(Player *player){

	int aux = -1;

	/* Inicializa os stas do jogador com os stats base */
	(*player).hp = BASE_HP;
	(*player).defense = BASE_DEF;
	(*player).attack = BASE_ATTACK;
	(*player).level = 1;
	(*player).XP = 0;
	(*player).NextLevel = BASE_NEXT_LEVEL;
	(*player).MaxHP = BASE_HP;

	srand(time(NULL));

	/* Cria a posicao que o jogador sera colocado no mapa */
	while((aux < 1) || (aux >= TAM - 1))
		aux = rand() % TAM;	

	(*player).y = aux;
	aux = -1;

	while((aux < 1) || (aux >= TAM - 1))
		aux = rand() % TAM;

	(*player).x = aux;
}

void enemyInit(Enemy *enemy){

	/* Inicializa os inimigos com seus stats base */

	(*enemy).hp = ENEMY_1_HP;
	(*enemy).defense = ENEMY_1_BASE_DEF;
	(*enemy).attack = ENEMY_1_BASE_ATTACK;
	(*enemy).givenXP = ENEMY_1_XP;
}

/* Inicializa o mapa */
void mapInit(Map map[TAM][TAM], int linha, int coluna, Enemy *enemies){

	int i, j, aux1 = -1, aux2 = -1, flag;

	/* Inicializa as beiradas do mapa com muros */
	for(i = 0; i < TAM; i++){
		map[0][i].wall = 1;
		map[TAM - 1][i].wall = 1;
		map[i][0].wall = 1;
		map[i][TAM - 1].wall = 1;

		map[0][i].used = 1;
		map[TAM - 1][i].used = 1;
		map[i][0].used = 1;
		map[i][TAM - 1].used = 1;

		map[0][i].player = 0;
		map[TAM - 1][i].player = 0;
		map[i][0].player = 0;
		map[i][TAM - 1].player = 0;

		map[0][i].enemyIndice = -1;
		map[TAM - 1][i].enemyIndice = -1;
		map[i][0].enemyIndice = -1;
		map[i][TAM - 1].enemyIndice = -1;
	}

	/* Inicializa o restantes das posicoes como vazias */
	for(i = 1; i < TAM - 1; i++)
		for(j = 1; j < TAM - 1; j++){
			map[i][j].wall = 0;
			map[i][j].player = 0;
			map[i][j].used = 0;
			map[i][j].enemyIndice = -1;
		}	

	/* Insere o jogador no mapa */
	map[linha][coluna].player = 1;
	map[linha][coluna].used = 1;

	/* Insere os inimigos no mapa */
	for(i = 0; i < NUM_INIMIGOS; i++){
		flag = 1;
		
		while(flag){
			aux1 = -1;
			aux2 = -1;

			while(((aux1 < 1) || (aux1 >= TAM - 1)) && (flag))
				aux1 = rand() % TAM;

			while(((aux2 < 1) || (aux2 >= TAM - 1)) && (flag))
				aux2 = rand() % TAM;

			if(map[aux1][aux2].used == 0){
				enemies[i].y = aux1;
				enemies[i].x = aux2;

				map[aux1][aux2].used = 1;
				map[aux1][aux2].enemyIndice = i;
				flag = 0;

				enemyInit(&enemies[i]);
			}
		}	
	}
}

/* Verfica se existe e carrega partida salva */
int gameLoad(Player *player, Map map[TAM][TAM], Enemy *enemies){

	char read;
	FILE *arq;

	arq = fopen("data.bin", "r");

	/* Caso o arquivo de salvar o jogo exista, permite que o jogo seja carregado *
	 * caso contrario retorna 0 e cria um novo mapa */
	if(arq != NULL){
		printf("Voce deseja continuar o jogo salvo? (y/n)\n");
		scanf(" %c", &read);

		if(read == 'y'){
			fread(map, sizeof(Map), TAM * TAM, arq);
			fread(player, sizeof(Player), 1, arq);
			fread(enemies, sizeof(Enemy), NUM_INIMIGOS, arq);
	
			fclose(arq);
			remove("data.bin");
			system("clear");
			return 1;
		}

		fclose(arq);	
	}

	return 0;
}
