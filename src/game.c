#include "game.h"
#include "stdio.h"
void game_init(Game *g)
{
    *g = (Game){.state = OPPONENT, .list = NULL};
    game_add_position(g);
    board_clear(&g->list->board);
}

void game_cut(Game *g)
{
    game_list_fwdestroy(g->list->next);
    g->list->next = NULL;
    g->state = MATCH;
}

static void game_add_position(Game *g)
{
    BoardList *root = (BoardList *)malloc(sizeof(BoardList));
    if (g->list != NULL)
    {
        g->list->next = root;
        root->board = g->list->board; // why can I do that????????????????????
    }
    else
    {
        root->board = board_init(800);
    }
    root->former = g->list;
    root->next = NULL;
    g->list = root;
}

void game_player_event(Game *g, pos p)
{
    if (pos_hover((pos){BOARDX, BOARDY}, (pos){BOARDLENGTH, BOARDLENGTH}, p))
    {
        int i = (p.x - BOARDX) / (TILESIZE + 1);
        int j = (p.y - BOARDY) / (TILESIZE + 1);

        if (g->list->board.disks[j][i] == VALID)
        {
            game_add_position(g);

            g->list->board.points[g->list->board.side]++;

            board_put_disk(&g->list->board, (pos){i, j});
            board_after_move(&g->list->board);
        }
    }
}

void game_AI_event(Game *g)
{
    game_add_position(g);
    minimax(&g->list->board, 0, INT32_MIN, INT32_MAX);
    board_after_move(&g->list->board);
}

static void game_listcpy(BoardList **dst, BoardList *src)
{
    if (src != NULL)
    {
        *dst = (BoardList *)malloc(sizeof(BoardList));
        (*dst)->board = src->board;
        (*dst)->next = NULL;
        src = src->former;

        while (src != NULL)
        {
            (*dst)->former = (BoardList *)malloc(sizeof(BoardList));
            (*dst)->former->board = src->board;
            (*dst)->former->next = *dst;
            *dst = (*dst)->former;
            src = src->former;
        }
        (*dst)->former = NULL;
        while ((*dst)->next != NULL)
            *dst = (*dst)->next;
    }
    else
    {
        *dst = NULL;
    }
}

void game_tolast(BoardList **list)
{
    while ((*list)->next != NULL)
        *list = (*list)->next;
}

void game_cpy(Game *dst, Game *src)
{
    *dst = (Game){.state = src->state, .list = NULL, .player_color = src->player_color, .opponent = src->opponent};
    game_listcpy(&dst->list, src->list);
}

void game_list_bwdestroy(BoardList *list)
{
    if (list == NULL)
        return;
    game_list_bwdestroy(list->former);
    free(list);
}

void game_list_fwdestroy(BoardList *list)
{
    if (list == NULL)
        return;
    game_list_fwdestroy(list->next);
    free(list);
}

void game_destroy(Game *g)
{
    game_tolast(&g->list);
    game_list_bwdestroy(g->list);
    free(g);
}