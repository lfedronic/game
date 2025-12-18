#include "stdlib.h"
#include "stdio.h"
#include "alg.h"

#define MAX_MOVES 8
#define c_c_r(x) fprintf(stdout, "<%s> successfully reached\n", (x));
typedef struct coord
{
    int x;
    int y;
} coord_t;

int get_rndm_int(int min, int max)
{
    int limit = RAND_MAX - (RAND_MAX % max);
    int r;
    do
    {
        r = rand();
    } while (r >= limit);
    return (r % max) + min;
}

move_t *rndm_alg(board_t *board, player_t *p)
{
    move_t *chosen_moves = malloc(sizeof(move_t) * p->n_snakes);
    int m = get_rows(board);
    int n = get_cols(board);

    if (is_setup_phase())
    {
        int x = 0;
        size_t count = (size_t)m * (size_t)n / 2;
        coord_t *egg_coords = malloc(count * sizeof(coord_t));
        if (p->is_p1)
        {
            for (int i = 0; i < m; i++)
            {
                for (int j = n - 1; j > i; j--)
                {
                    coord_t c = {i, j};
                    egg_coords[x] = c;
                    x++;
                }
            }
        }
        else
        {
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < i; j++)
                {
                    coord_t c = {i, j};
                    egg_coords[x] = c;
                    x++;
                }
            }
        }

        for (int i = 0; i < p->n_snakes; i++)
        {
            int coord_id = get_rndm_int(0, x);
            move_t m = {&(p->snakes[i]), egg_coords[coord_id].x, egg_coords[coord_id].y, false};
            chosen_moves[i] = m;
        }
    }
    else
    {
        move_t *available_moves = malloc(sizeof(move_t) * MAX_MOVES);
        board_t *board_cpy = cpy_board(board);
        uint8_t tmp = 0b00001001;
        tmp |= (uint8_t)p->is_p1 << 2;
        for (int i = 0; i < p->n_snakes; i++)
        {
            int n = load_available_moves_for_snake(board_cpy, &(p->snakes[i]), available_moves);
            int move_id = get_rndm_int(0, n);
            chosen_moves[i] = available_moves[move_id];
            // update board: is_snake, correctly set player bit, isn't egg, is_head
            board_cpy->grid[chosen_moves[i].x][chosen_moves[i].y] = tmp;
        }
    }
    return chosen_moves;
}
