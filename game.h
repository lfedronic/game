#ifndef GAME_H
#define GAME_H
#include "stdlib.h"
#include "board.h"
#include "player.h"

typedef struct coord coord_t;

int load_available_moves(board_t *board, player_t *p, coord_t *moves);

void init_game(board_t *board, player_t *p0, player_t *p1);

int take_turn(board_t *board, player_t *p);

void p_move(coord_t *moves, int n_moves);

coord_t *copy_moves(coord_t *moves, int n_moves);

void play_move_history(int fps);

#endif
