#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "time.h"
#include "string.h"
#include "board.h"
#include "player.h"
#include "game.h"
#include "alg.h"

#define MAX_ITERS 500

int main()
{
    int p0_wins = 0;
    int p1_wins = 0;
    char *p0_name = "dfs_demon";
    char *p1_name = "rando_mando";
    srand((unsigned)time(NULL));
    // for (int i = 0; i < MAX_ITERS; i++)
    // {
    uint16_t m = 21;
    uint16_t n = 21;
    board_t *board = init_board(m, n);

    player_t *p0 = init_player(m - 1, 0, 'O', p0_name);
    player_t *p1 = init_player(0, n - 1, 'X', p1_name);

    p0->alg = &dfs_alg;
    p1->alg = &rndm_alg;

    init_game(board, p0, p1);

    take_turn(board, p0);
    while (!p0->done || !p1->done)
    {
        take_turn(board, p1);
        take_turn(board, p1);
        if (p1->done)
        {
            while (!p0->done)
            {
                take_turn(board, p0);
            }
            break;
        }
        take_turn(board, p0);
        take_turn(board, p0);

        if (p0->done)
        {
            while (!p1->done)
            {
                take_turn(board, p1);
            }
            break;
        }
    }
    p_board(board);
    play_move_history(1200);
    printf("\n\nGame over!\n");
    if (p0->tiles > p1->tiles)
    {
        printf("p0 (%s) wins with %d tiles to p1 (%s's) %d!\n", p0->name, p0->tiles, p1->name, p1->tiles);
        p0_wins++;
    }
    else if (p0->tiles < p1->tiles)
    {
        printf("p1 (%s) wins with %d tiles to p0 (%s's) %d!\n", p1->name, p1->tiles, p0->name, p0->tiles);
        p1_wins++;
    }
    else
    {
        printf("The game finishes in a draw!\n");
    }
    free_board(board);
    retire_player(p0);
    retire_player(p1);
    // }
    // printf("\n\n\nAfter %d games, ", MAX_ITERS);
    // if (p0_wins > p1_wins)
    // {
    //     printf("p0 (%s) beat p1 (%s) %.2f%% of the time.\n", p0_name, p1_name, (p0_wins / (double)MAX_ITERS) * 100);
    // }
    // else if (p1_wins > p0_wins)
    // {
    //     printf("p1 (%s) beat p0 (%s) in %.2f%% of the time.\n", p1_name, p0_name, (p1_wins / (double)MAX_ITERS) * 100);
    // }
    // else
    // {
    //     printf("p0 and p1 drew.\n");
    // }
}