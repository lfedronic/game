#ifndef PLAYER_H
#define PLAYER_H

#include "stdlib.h"
#include "board.h"
#include "game.h"

typedef struct player player_t;
typedef struct move move_t;
typedef struct snake
{
    int hx;
    int hy;
    player_t *p;
} snake_t;
typedef struct player
{
    char *name;
    bool is_p1;
    move_t *(*alg)(board_t *, struct player *p);
    int n_snakes;
    snake_t *snakes;
    int n_eggs;
} player_t;

player_t *init_player(char *name, bool is_p1, int n_snakes);

void p_player(player_t *p);
void assign_alg(player_t *p, move_t *(*alg)(board_t *, struct player *p));
player_t *cpy_player(player_t *p);
char *get_player_name(player_t *);
void retire_player(player_t *);

#endif