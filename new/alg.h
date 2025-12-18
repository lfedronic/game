#ifndef ALG_H
#define ALG_H

#include "stdlib.h"
#include "board.h"
#include "player.h"
#include "game.h"

move_t *rndm_alg(board_t *board, player_t *p);
// coord_t dfs_alg(board_t *board, player_t *p, uint16_t n_moves, coord_t *moves);
// coord_t spicy_dfs_alg(board_t *board, player_t *p, uint16_t n_moves, coord_t *moves);

#endif