#include "stdlib.h"
#include "stdio.h"
#include "game.h"
#include "board.h"
#include "player.h"

#ifdef _WIN32
#include <windows.h>
static void sleep_ms(unsigned ms) { Sleep(ms); }
#else
#include <time.h>
static void sleep_ms(unsigned ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}
#endif

#define MAX_AVAILABLE_MOVES 8
#define HEAD_CHAR '*'

board_t **move_history;
int move_history_id;

void init_game(board_t *board, player_t *p0, player_t *p1)
{
    // printf("Initializing game!\n");
    board->grid[p0->head_x][p0->head_y] = p0->sign;
    board->grid[p1->head_x][p1->head_y] = p1->sign;
    move_history = malloc(sizeof(board_t *) * board->m * board->n);
    move_history[0] = copy_board(board);
    move_history_id = 1;
}

void p_move(coord_t *moves, int n_moves)
{
    for (int i = 0; i < n_moves; i++)
    {
        printf("Move #%d: (%d, %d)\n", i, moves[i].x, moves[i].y);
    }
}

int load_available_moves(board_t *board, player_t *p, coord_t *moves)
{

    int16_t head_x = p->head_x;
    int16_t head_y = p->head_y;
    coord_t move;
    uint16_t next = 0;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int16_t x = head_x + dx;
            int16_t y = head_y + dy;
            if (x < 0 || x >= board->m || y < 0 || y >= board->n)
            {
                continue;
            }
            if (!board->grid[x][y])
            {
                move.x = x;
                move.y = y;
                moves[next] = move;
                next++;
            }
        }
    }
    return next; // num moves
}

int take_turn(board_t *board, player_t *p)
{
    // printf("\nPlayer %s turn: \n", p->name);
    coord_t *moves = malloc(sizeof(coord_t) * MAX_AVAILABLE_MOVES);
    int n_moves = load_available_moves(board, p, moves);
    if (n_moves == 0)
    {
        p->done = true;
        // printf("Player %s done! \n", p->name);
        return 0;
    }
    board->grid[p->head_x][p->head_y] = p->sign; // reset head (*) marker to normal sign
    board_t *board_copy = copy_board(board);
    coord_t move = p->alg(board_copy, p, n_moves, moves);
    if (move.x < 0 || move.x >= board->m || move.y < 0 || move.y >= board->n)
    {
        // printf("Invalid move.\n");
        return 1;
    }
    if (!board->grid[move.x][move.y])
    {
        board->grid[move.x][move.y] = HEAD_CHAR;
        p->head_x = move.x;
        p->head_y = move.y;
        move_history[move_history_id] = copy_board(board);
        move_history_id++;
        p->tiles++;
        return 0;
    }
    return 1;
}

coord_t *copy_moves(coord_t *moves, int n_moves)
{
    coord_t *moves_copy = malloc(sizeof(coord_t) * n_moves);
    for (int i = 0; i < n_moves; i++)
    {
        moves_copy[i].x = moves[i].x;
        moves_copy[i].y = moves[i].y;
    }
    return moves_copy;
}

void play_move_history(int fps)
{
    printf("\033[2J\033[H"); // clear screen, cursor to home
    for (int i = 0; i < move_history_id; i++)
    {
        p_board(move_history[i]);
        free_board(move_history[i]);
        sleep_ms((60 / (double)fps) * 1000);
        printf("\033[2J\033[H"); // clear screen, cursor to home
        // fflush(stdout);
    }
}