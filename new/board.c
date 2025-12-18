#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "board.h"
#include "stdio.h"

#define EMPTY_TILE '-'
#define HEAD_TILE '*'
#define EGG_TILE 'o'
#define P0_TILE 'O'
#define P1_TILE 'X'

#define EGG_HEAD_CHECKER 0b00000011

board_t *init_board(int rows, int cols)
{
    board_t *board = malloc(sizeof(board_t));
    board->rows = rows;
    board->cols = cols;
    board->grid = malloc(rows * sizeof(uint8_t *));
    for (int i = 0; i < rows; i++)
    {
        board->grid[i] = calloc(cols, sizeof(uint8_t));
    }
    return board;
}

void molt_board(board_t *board)
{
    for (int i = 0; i < board->rows; i++)
    {
        for (int j = 0; j < board->cols; j++)
        {
            // set all non-egg, non-head player tiles to empty tiles
            uint8_t *tile = &(board->grid[i][j]);
            if (((*tile >> 3) & 1u) && !(EGG_HEAD_CHECKER & *tile))
            {
                *tile = 0;
            }
        }
    }
}

void free_board(board_t *board)
{
    for (int i = 0; i < board->rows; i++)
    {
        free(board->grid[i]);
    }
    free(board->grid);
    free(board);
}

int get_rows(board_t *board)
{
    return board->rows;
}

int get_cols(board_t *board)
{
    return board->cols;
}

void p_board(board_t *board)
{
    for (int i = 0; i < board->rows; i++)
    {
        for (int j = 0; j < board->cols; j++)
        {
            char sym;
            uint8_t tile = board->grid[i][j];
            if (!tile)
            {
                sym = EMPTY_TILE;
            }
            else if ((tile >> 1) & 1u)
            {
                sym = EGG_TILE;
            }
            else if (tile & 1u)
            {
                sym = HEAD_TILE;
            }
            else if ((tile >> 2) & 1u)
            {
                sym = P1_TILE;
            }
            else
            {
                sym = P0_TILE;
            }
            printf("%c ", sym);
        }
        printf("\n");
    }
}

board_t *cpy_board(board_t *board)
{
    board_t *board_cpy = malloc(sizeof(board_t));
    board_cpy->rows = board->rows;
    board_cpy->cols = board->cols;
    board_cpy->grid = malloc(board->rows * sizeof(uint8_t *));
    for (int i = 0; i < board->rows; i++)
    {
        board_cpy->grid[i] = malloc(board->cols * sizeof(uint8_t));
        memcpy(board_cpy->grid[i], board->grid[i], board->cols);
    }
    return board_cpy;
}
