#ifndef BOARD_H
#define BOARD_H

#include "stdlib.h"

typedef struct board
{
    uint16_t m;
    uint16_t n;
    char **grid;
} board_t;

typedef struct coord
{
    uint16_t x;
    uint16_t y;
} coord_t;

board_t *init_board(uint16_t m, uint16_t n);

void free_board(board_t *board);

void p_board(board_t *board);

board_t *copy_board(board_t *board);

#endif
