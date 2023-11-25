#include "game.h"
#include "stdio.h"
void game_init(Game *g, Master *m)
{
    *g = (Game){.state = OPPONENT, .list = NULL};
    game_add_position(g, m);
    board_make(&g->list->board, m);
}

void game_add_position(Game *g, Master *m)
{
    BoardList *root = (BoardList *)malloc(sizeof(BoardList));
    if (g->list != NULL)
    {
        g->list->next = root;
        root->board = g->list->board; //why can I do that????????????????????
    }
    else
    {
        root->board = board_init(800, m);
    }
    root->former = g->list;
    root->next = NULL;
    g->list = root;

    g->history_board = g->list;
}

bool game_player_event(Game *g, int x, int y, Master *m)
{
    if (!pos_hover((pos){g->list->board.position.x, g->list->board.position.y}, (pos){g->list->board.length, g->list->board.length}, (pos){x, y}))
    {
        return false;
    }

    int i = (x - g->list->board.position.x) / (g->list->board.tile_size + 1);
    int j = (y - g->list->board.position.y) / (g->list->board.tile_size + 1);

    // 0<i<8, 0<j<8
    if (g->list->board.disks[j][i].color == VALID)
    {
        game_add_position(g, m);

        // g->list->board.state = BASIC;
        g->list->board.points[g->list->board.side]++;

        board_put_disk(&g->list->board, (pos){i, j}, m);

        board_after_move(&g->list->board, m);
        return true;
    }
    return false;
}

void game_AI_event(Game *g, Master *m)
{
    game_add_position(g, m);
    minimax(&g->list->board, 0, INT32_MIN, INT32_MAX, m);
    board_after_move(&g->list->board, m);
}

void game_listcpy(BoardList **dst, BoardList *src)
{
    if (src != NULL)
    {
        *dst = (BoardList *)malloc(sizeof(BoardList));
        (*dst)->board = src->board;
        (*dst)->next = NULL;
        src = src->former;

        while (src != NULL)
        {
            (*dst)->former = (BoardList*)malloc(sizeof(BoardList));
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

void game_tofirst(BoardList **list)
{
    while ((*list)->next != NULL)
        *list = (*list)->next;
}

void game_hbcpy(Game *dst, Game *src)
{
    BoardList *temp = src->list;
    while (src->history_board != temp)
    {
        temp = temp->former;
        dst->history_board = dst->history_board->former;
    }
}

void game_cpy(Game *dst, Game *src)
{
    *dst = (Game){.state = src->state, .list = NULL, .player_color = src->player_color, .opponent = src->opponent};
    game_listcpy(&dst->list, src->list);
    dst->history_board = dst->list;
    game_hbcpy(dst, src);
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
    game_list_bwdestroy(g->list);
    free(g);
}