#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "player.h"
#include "board.h"

#define c_c_r(x) fprintf(stdout, "<%s> successfully reached\n", (x));

player_t *init_player(char *name, bool is_p1, int n_snakes)
{
    player_t *p = malloc(sizeof(player_t));
    size_t n = strlen(name) + 1;
    p->name = malloc(n);
    memcpy(p->name, name, n);
    p->is_p1 = is_p1;
    p->n_snakes = n_snakes;
    p->n_eggs = n_snakes;
    snake_t *snakes = malloc(n_snakes * sizeof(snake_t));
    for (int i = 0; i < n_snakes; i++)
    {
        snakes[i].hx = 0;
        snakes[i].hy = 0;
        snakes[i].p = p;
    }
    p->snakes = snakes;
    return p;
}

void assign_alg(player_t *p, move_t *(*alg)(board_t *, struct player *p))
{
    p->alg = alg;
}

void p_player(player_t *p)
{
    printf("Name: %s, is_p1: %d\n", p->name, (int)p->is_p1);
    printf("n_snakes: %d, n_eggs %d\n", p->n_snakes, p->n_eggs);
    for (int i = 0; i < p->n_snakes; i++)
    {
        snake_t s = p->snakes[i];
        printf("snake #%d head: (%d, %d)\n", i, s.hx, s.hy);
    }
}

player_t *cpy_player(player_t *p)
{
    player_t *p_cpy = malloc(sizeof(player_t));
    p_cpy->name = malloc(strlen(p->name) * sizeof(char));
    memcpy(p_cpy->name, p->name, strlen(p->name));
    p_cpy->is_p1 = p->is_p1;
    p_cpy->alg = p->alg;
    p_cpy->n_snakes = p->n_snakes;
    p_cpy->snakes = malloc(sizeof(snake_t));
    p_cpy->n_eggs = p->n_snakes;
    for (int i = 0; i < p->n_snakes; i++)
    {
        p_cpy->snakes[i].p = malloc(sizeof(snake_t));
        p_cpy->snakes[i].p = p_cpy;
        p_cpy->snakes[i].hx = p->snakes[i].hx;
        p_cpy->snakes[i].hy = p->snakes[i].hy;
    }
    return p_cpy;
}

char *get_player_name(player_t *p)
{
    return p->name;
}

void retire_player(player_t *p)
{
    free(p->name);
    free(p->snakes);
    free(p);
}
