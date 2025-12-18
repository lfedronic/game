#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "board.h"
#include "stdio.h"

#define EMPTY_TILE '-'

board_t *init_board(uint16_t m, uint16_t n)
{
    board_t *board = malloc(sizeof(board_t));
    board->m = m;
    board->n = n;
    board->grid = malloc(m * sizeof(char *));
    for (int i = 0; i < m; i++)
    {
        board->grid[i] = calloc(n, sizeof(char));
    }
    return board;
}

void free_board(board_t *board)
{
    for (int i = 0; i < board->m; i++)
    {
        free(board->grid[i]);
    }
    free(board->grid);
    free(board);
}

void p_board(board_t *board)
{
    for (int i = 0; i < board->m; i++)
    {
        for (int j = 0; j < board->n; j++)
        {
            if (!board->grid[i][j])
            {
                printf("%c ", EMPTY_TILE);
            }
            else
            {
                printf("%c ", board->grid[i][j]);
            }
        }
        printf("\n");
    }
}

board_t *copy_board(board_t *board)
{
    board_t *board_copy = malloc(sizeof(board_t));
    board_copy->m = board->m;
    board_copy->n = board->n;
    board_copy->grid = malloc(board->m * sizeof(char *));
    for (int i = 0; i < board->m; i++)
    {
        board_copy->grid[i] = malloc(board->n * sizeof(char));
        memcpy(board_copy->grid[i], board->grid[i], board->n);
    }
    return board_copy;
}
