#include "stdlib.h"
#include "stdbool.h"
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

#define c_c_r(x) fprintf(stdout, "<%s> successfully reached\n", (x));

#define MAX_AVAILABLE_MOVES 8
#define MOVES_PER_TURN 3
#define NUM_PLAYERS 2
#define TURNS_PER_MOLT 10

game_t *game;

void init_game(board_t *board, player_t *p0, player_t *p1)
{
    game = malloc(sizeof(game_t));
    game->setup_phase = true;
    game->done = false;
    game->board = board;
    game->turns = 0;
    game->players = malloc(sizeof(player_t *) * NUM_PLAYERS);
    game->players[0] = p0;
    game->players[1] = p1;
    game->move_history = malloc(sizeof(board_t *) * board->rows * board->cols);
    game->move_history[0] = cpy_board(board);
    game->move_history_id = 1;
}

void p_game()
{
    printf("Board: %d by %d\n", game->board->rows, game->board->cols);
    printf("Players: %s vs %s\n", game->players[0]->name, game->players[1]->name);
    printf("Set-up phase: %d\n", (int)game->done);
    printf("Turn: %d\n", game->turns);
}

void free_resources()
{
    free_board(game->board);
    retire_player(game->players[0]);
    retire_player(game->players[1]);
    free(game->players);
    free(game->move_history);
}

player_t *get_winner()
{
    return game->players[game->p1_wins];
}

bool is_setup_phase()
{
    return game->setup_phase;
}

bool is_over()
{
    return game->done;
}

int load_available_moves_for_snake(board_t *board, snake_t *snake, move_t *moves)
{
    int hx = snake->hx;
    int hy = snake->hy;
    move_t move;
    uint8_t next = 0;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int x = hx + dx;
            int y = hy + dy;
            if (x < 0 || x >= board->rows || y < 0 || y >= board->cols)
            {
                continue;
            }
            uint8_t tile = board->grid[x][y];
            if (!tile)
            {
                move.snake = snake;
                move.x = x;
                move.y = y;
                move.captures_egg = false;
                moves[next] = move;
                next++;
            }
            else if ((tile >> 1) & 1u) // tile is egg (opponents')
            {
                if (snake->p->is_p1 ^ ((tile >> 2) & 1u))
                {
                    move.snake = snake;
                    move.x = x;
                    move.y = y;
                    move.captures_egg = true;
                    moves[next] = move;
                    next++;
                }
            }
        }
    }
    return next; // num moves
}

// currently, assumes the move can be reached from snakes head
bool is_valid(board_t *board, player_t *p, move_t *moves)
{
    for (int i = 0; i < MOVES_PER_TURN; i++)
    {
        int x = moves[i].x;
        int y = moves[i].y;
        if (x < 0 || x >= board->rows || y < 0 || y >= board->cols)
        {
            return false;
        }
        uint8_t tile = board->grid[x][y];
        // is own egg
        if (((tile >> 1) & 1u) && !(((tile >> 2) & 1u) ^ (uint8_t)p->is_p1))
        {
            c_c_r("own head");
            return false;
        }
        else if ((tile >> 1) & 1u)
        {
            continue;
        }
        else if ((tile >> 3))
        { // is_snake or is_head
            c_c_r("snake");
            return false;
        }
    }
    return true;
}

bool take_turn(board_t *board, player_t *p)
{
    game->turns++;
    if (game->turns % TURNS_PER_MOLT == 0)
    {
        molt_board(board);
    }
    move_t *moves = malloc(sizeof(move_t) * MOVES_PER_TURN);
    moves = p->alg(board, p);
    // p_moves(moves, MOVES_PER_TURN);
    bool p1 = p->is_p1;
    // TODO: validate setup phase moves too + complete validation in general
    if (game->setup_phase)
    {
        bool p1 = p->is_p1;
        uint8_t tmp = 0b00001011;
        tmp |= (uint8_t)p1 << 2;
        for (int i = 0; i < MOVES_PER_TURN; i++)
        {
            moves[i].snake->hx = moves[i].x; // update head
            moves[i].snake->hy = moves[i].y;
            board->grid[moves[i].x][moves[i].y] = tmp;
        }
        if (game->turns == 2)
        {
            game->setup_phase = false;
        }
    }
    else
    {
        if (!is_valid(board, p, moves))
        {
            p_moves(moves, MOVES_PER_TURN);
            p_board(board);
            return false;
        }
        for (int i = 0; i < MOVES_PER_TURN; i++)
        {
            board->grid[moves[i].snake->hx][moves[i].snake->hy] -= 1; // reset is_head to 0
            moves[i].snake->hx = moves[i].x;                          // update head
            moves[i].snake->hy = moves[i].y;
            // is_snake, correctly set player bit, isn't egg, is_head
            uint8_t tmp = 0b00001001;
            tmp |= ((uint8_t)p1 << 2);
            board->grid[moves[i].x][moves[i].y] = tmp;
            if (moves[i].captures_egg)
            {
                player_t *p = game->players[!p1]; // player's opponent
                if (p->n_eggs - 1 <= 0)           // victory condition
                {
                    game->done = true;
                    game->p1_wins = p1;
                }
                p->n_eggs -= 1;
            }
        }
    }
    game->move_history[game->move_history_id] = cpy_board(board);
    game->move_history_id++;
    return true;
}
// 1001

void p_moves(move_t *moves, int n_moves)
{
    for (int i = 0; i < n_moves; i++)
    {
        printf("#%d: (%d, %d), \n", i, moves[i].x, moves[i].y);
    }
}

void play_move_history(int fps)
{
    printf("\033[2J\033[H"); // clear screen, cursor to home
    for (int i = 0; i < game->move_history_id; i++)
    {
        p_board(game->move_history[i]);
        free_board(game->move_history[i]);
        sleep_ms((60 / (double)fps) * 1000);
        printf("\033[2J\033[H"); // clear screen, cursor to home
        // fflush(stdout);
    }
}