/* Author: ETA Team *
 * Last Modification: 07/07/2015 by Foo */

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

/* Funcao que recebe XP e verifica se o personagem upou */
void verificaXP(Player *player, int xpGanho){

	(*player).XP += xpGanho;

	/* Verifica se o jogador upou e atualiza os stats do jogador */
	if((*player).XP >= (*player).NextLevel){
		(*player).XP -= (*player).NextLevel;
		(*player).level++;
		(*player).attack += 1.5 * (*player).level * BASE_ATTACK / 10;
		(*player).defense += (*player).level * BASE_DEF / 5;
		(*player).MaxHP += (*player).level * BASE_HP / 10; 
		(*player).hp += (*player).level * BASE_HP / 5;
		(*player).NextLevel *= 2;
	}
}

/* Funcao que executa o ataque de inimigos */
int enemyAttack(Player *player, Enemy *enemy){

	int damageTaken;

	damageTaken = (*enemy).attack - (*player).defense;
	(*player).hp -= damageTaken;

	printf("Inimigo causou a voce %d de dano!\n", damageTaken);

	if((*player).hp <= 0){
		printf("Voce morreu!!\n\nGame Over baby!!!!\n\n\n");
		return 0;
	}
	
	return 1;	
}

/* Funcao que movimenta os inimigos */
int enemyAction(Player *player, Map map[TAM][TAM], Enemy *enemies){

	int i, aux, x, y, flag;

	srand(time(NULL));

	/* Para cada inimigo */
	for(i = 0; i < NUM_INIMIGOS; i++){
		if(enemies[i].hp > 0){
			x = enemies[i].x;
			y = enemies[i].y;
			flag = 1;

			/* Verifica se o player esta em uma regiao adjacente e o ataca */
			if(map[y + 1][x].player){
				flag = 0;

				if(enemyAttack(player, &enemies[i]) == 0)
					return 0;
			}

			else if((flag) && (map[y - 1][x].player)){
				flag = 0;

				if(enemyAttack(player, &enemies[i]) == 0)
					return 0;
			}

			else if((flag) && (map[y][x - 1].player)){
				flag = 0;

				if(enemyAttack(player, &enemies[i]) == 0)
					return 0;
			}	

			else if((flag) && (map[y][x + 1].player)){
				flag = 0;

				if(enemyAttack(player, &enemies[i]) == 0)
					return 0;
			}

			/* Caso nao esteja, movimenta aleatorioamente o inimigo */
			if(flag){
				aux = rand();

				if(aux % 4 == 3){

					if(map[y - 1][x].used == 0){
						map[y][x].used = 0;
						map[y][x].enemyIndice = -1;

						map[y - 1][x].used = 1;
						map[y - 1][x].enemyIndice = i;

						enemies[i].y--;
					}
				}

				else if(aux % 4 == 2){

					if(map[y + 1][x].used == 0){
						map[y][x].used = 0;
						map[y][x].enemyIndice = -1;

						map[y + 1][x].used = 1;
						map[y + 1][x].enemyIndice = i;

						enemies[i].y++;
					}
				}

				if(aux % 4 == 1){

					if(map[y][x - 1].used == 0){
						map[y][x].used = 0;
						map[y][x].enemyIndice = -1;

						map[y][x - 1].used = 1;
						map[y][x - 1].enemyIndice = i;

						enemies[i].x--;
					}
				}

				if(aux % 4 == 0){

					if(map[y][x + 1].used == 0){
						map[y][x].used = 0;
						map[y][x].enemyIndice = -1;

						map[y][x + 1].used = 1;
						map[y][x + 1].enemyIndice = i;

						enemies[i].x++;
					}
				}
			}	
		}	
	}	

	return 1;
}

/* Funcao que executa o ataque do player */
void combate(Player *player, Enemy *enemy, Map *position){

	int damageDone;

	/* Calcula o dano causado e o dano infringido, verifica se o inimigo morreu e *
	 * atualiza as posicoes no mapa de acordo com o resultado */
	damageDone = (*player).attack - (*enemy).defense;

	(*enemy).hp -= damageDone;
	printf("Voce causou a inimigo %d de dano!\n", damageDone);

	if((*enemy).hp <= 0){
		printf("Inimigo morreu!\n");
		printf("Voce recebeu %d de XP!\n", (*enemy).givenXP);
		(*position).enemyIndice = -1;
		(*position).used = 0;

		verificaXP(player, (*enemy).givenXP);
	}

	if((*enemy).hp > 0)
		printf("Inimigo possui %d de vida!\n", (*enemy).hp);
}