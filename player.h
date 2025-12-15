#ifndef PLAYER_H
#define PLAYER_H

#include "stdlib.h"
#include "board.h"
#include "stdbool.h"

typedef struct player
{
    char *name;
    char sign;
    uint16_t tiles;
    int16_t head_x;
    int16_t head_y;
    coord_t (*alg)(board_t *, struct player *p, uint16_t n_moves, coord_t *moves);
    bool done;
} player_t;

player_t *init_player(uint16_t start_x, uint16_t start_y, char sign, char *name);

void retire_player(player_t *p);

player_t *copy_player(player_t *p);

#endif