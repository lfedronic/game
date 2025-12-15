#include "stdlib.h"
#include "string.h"
#include "player.h"
#include "board.h"

player_t *init_player(uint16_t start_x, uint16_t start_y, char sign, char *name)
{
    player_t *p = malloc(sizeof(player_t));
    p->head_x = start_x;
    p->head_y = start_y;
    p->sign = sign;
    p->done = false;
    size_t n = strlen(name) + 1;
    p->name = malloc(n);
    memcpy(p->name, name, n);
    return p;
}

player_t *copy_player(player_t *p)
{
    player_t *p_copy = malloc(sizeof(player_t));
    p_copy->alg = p->alg;
    p_copy->done = p->done;
    p_copy->head_x = p->head_x;
    p_copy->head_y = p->head_y;
    p_copy->sign = p->sign;
    p_copy->tiles = p->tiles;
    p_copy->name = malloc(strlen(p->name) * sizeof(char));
    memcpy(p_copy->name, p->name, strlen(p->name));
    return p_copy;
}

void retire_player(player_t *p)
{
    free(p->name);
    free(p);
}
