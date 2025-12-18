#ifndef BOARD_H
#define BOARD_H
#include "stdlib.h"

typedef struct board
{
    int rows;
    int cols;
    uint8_t **grid;
} board_t;

board_t *init_board(int m, int n);

void molt_board(board_t *board);

void free_board(board_t *board);

int get_rows(board_t *board);

int get_cols(board_t *board);

void p_board(board_t *board);

board_t *cpy_board(board_t *board);
#endif