#include "stdlib.h"
#include "stdio.h"
#include "signal.h"
#include "time.h"
#include "board.h"
#include "player.h"
#include "game.h"
#include "alg.h"

#define FPS 600

void termination_handler(int signum)
{
    play_move_history(FPS);
    free_resources();
}

int main()
{
    srand((unsigned)time(NULL));
    signal(SIGINT, termination_handler);
    board_t *board = init_board(10, 10);
    player_t *p0 = init_player("rando_mando", false, 3);
    player_t *p1 = init_player("randa_amanda", true, 3);
    assign_alg(p0, rndm_alg);
    assign_alg(p1, rndm_alg);
    init_game(board, p0, p1);
    p_game();

    // use forward declare so that user can't modify these pointers
    while (!is_over())
    {
        if (!take_turn(board, p0))
        {
            printf("p0's move failed\n");
        }
        if (!is_over())
        {
            if (!take_turn(board, p1))
            {
                printf("p1's move failed\n");
            }
        }
    }

    player_t *winner = get_winner();
    printf("%s wins!", get_player_name(winner));
    play_move_history(FPS);
    free_resources();
}