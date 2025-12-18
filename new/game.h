#ifndef GAME_H
#define GAME_H
#include "stdlib.h"
#include "stdbool.h"
#include "board.h"
#include "player.h"

typedef struct player player_t;
typedef struct snake snake_t;

typedef struct move
{
    snake_t *snake;
    int x;
    int y;
    bool captures_egg;
} move_t;

typedef struct game
{
    bool setup_phase; // 1: placing eggs, 2: capturing eggs
    bool done;
    bool p1_wins;
    int turns;
    board_t *board;
    player_t **players;
    board_t **move_history;
    int move_history_id;

} game_t;

void init_game(board_t *board, player_t *p0, player_t *p1);

void p_game();

void free_resources();

player_t *get_winner();

bool is_setup_phase();

bool is_over();

int load_available_moves_for_snake(board_t *board, snake_t *snake, move_t *moves);

bool is_valid(board_t *board, player_t *p, move_t *moves);

bool take_turn(board_t *board, player_t *p);

void p_moves(move_t *moves, int n_moves);

move_t *copy_moves(move_t *moves, int n_moves);

void play_move_history(int fps);

#endif
