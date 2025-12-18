#include "stdlib.h"
#include "stdio.h"
#include "alg.h"

#define MAX_MOVES 8

typedef struct moves_after
{
    coord_t move;
    int most_moves;
} moves_after_t;

coord_t rndm_alg(board_t *board, player_t *p, uint16_t n_moves, coord_t *moves)
{

    int limit = RAND_MAX - (RAND_MAX % n_moves);
    int r;
    do
    {
        r = rand();
    } while (r >= limit);
    int move_id = (r % n_moves);

    // int move_id = (rand() % n_moves);
    return moves[move_id];
}

int dfs(board_t *board, player_t *p, int most_moves, int iters, const int MAX_ITERS)
{
    if (iters >= MAX_ITERS)
    {
        return most_moves;
    }
    coord_t *moves = malloc(sizeof(coord_t) * MAX_MOVES);
    int n_moves = load_available_moves(board, p, moves);
    if (n_moves == 0)
    {
        return most_moves;
    }
    // printf("Iter #%d, ", iters);
    // printf("Adding %d moves:\n", n_moves);
    // p_move(moves, n_moves);
    most_moves += n_moves;
    for (int i = 0; i < n_moves; i++)
    {
        coord_t move = moves[i];
        board->grid[move.x][move.y] = p->sign; // should put these 3 lines into a utility
        p->head_x = move.x;
        p->head_y = move.y;
        // board_t *board_copy = copy_board(board);
        most_moves = dfs(board, p, most_moves, iters + 1, MAX_ITERS);
    }
    free(moves);
    return most_moves;
}

coord_t dfs_alg(board_t *board, player_t *p, uint16_t n_moves, coord_t *moves)
{
    // p_move(moves, n_moves);
    moves_after_t *moves_after = malloc(sizeof(moves_after) * n_moves);
    coord_t best_move;
    int max_moves = 0;
    for (int i = 0; i < n_moves; i++)
    {
        board_t *board_copy = copy_board(board);
        player_t *p_copy = copy_player(p);
        p_copy->head_x = moves[i].x;
        p_copy->head_y = moves[i].y;
        int most_moves = dfs(board_copy, p_copy, 0, 0, 4);
        // printf("Exploring move#%d: (%d, %d). Found max of %d moves.\n", i, moves[i].x, moves[i].y, most_moves);
        moves_after[i].most_moves = most_moves;
        moves_after[i].move = moves[i];
        if (most_moves > max_moves)
        {
            max_moves = most_moves;
            best_move = moves_after[i].move;
        }
        free_board(board_copy);
        retire_player(p_copy);
    }
    return best_move;
}

coord_t spicy_dfs_alg(board_t *board, player_t *p, uint16_t n_moves, coord_t *moves)
{
    moves_after_t *moves_after = malloc(sizeof(moves_after) * n_moves);
    coord_t *best_moves = malloc(sizeof(coord_t) * n_moves);
    int max_moves = 0;
    int next = 0;
    for (int i = 0; i < n_moves; i++)
    {
        board_t *board_copy = copy_board(board);
        player_t *p_copy = copy_player(p);
        p_copy->head_x = moves[i].x;
        p_copy->head_y = moves[i].y;
        int most_moves = dfs(board_copy, p_copy, 0, 0, 2);
        // printf("Exploring move#%d: (%d, %d). Found max of %d moves.\n", i, moves[i].x, moves[i].y, most_moves);
        moves_after[i].most_moves = most_moves;
        moves_after[i].move = moves[i];
        if (most_moves >= max_moves)
        {
            max_moves = most_moves;
            best_moves[next] = moves[i];
            next++;
        }
        free_board(board_copy);
        retire_player(p_copy);
    }
    int move_id = (rand() % next);
    coord_t best_move = best_moves[move_id];
    free(best_moves);
    return best_move;
}