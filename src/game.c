#include "game.h"

Game game_init(disk_color player_c, Opponent opp)
{
    return (Game){.list = NULL, .player_color = player_c, .opponent = opp};
}

void game_add_position(Game *g, Master* m)
{
    BoardList *root = (BoardList *)malloc(sizeof(BoardList));
    if (g->list != NULL){
        g->list->next = root;
        root->board = g->list->board;
    }
    else{
        root->board = board_init(800, m);
    }
    root->former = g->list;
    root->next = NULL;
    g->list = root;

    g->history_board = g->list;
}

void game_player_event(Game *g, int x, int y, disk_color *side, Master *m, b_event *e)
{
    if (!pos_hover((pos){g->list->board.x, g->list->board.y}, (pos){g->list->board.length, g->list->board.length}, (pos){x, y}))
    {
        return;
    }

    int i = (x - g->list->board.x) / (g->list->board.tile_size + 1);
    int j = (y - g->list->board.y) / (g->list->board.tile_size + 1);

    if (g->list->board.disks[j][i].color == VALID)
    {
        game_add_position(g, m);

        *e = BASIC;
        g->list->board.side[*side]++;

        board_put_disk(&g->list->board, (pos){i, j}, m, *side);
        
        board_after_move(&g->list->board, side, m, e);
    }
}

void game_AI_event(Game *g, disk_color *side, Master *m, b_event *e)
{
    game_add_position(g, m);
    minimax(&g->list->board, 0, INT32_MIN, INT32_MAX, false, m);
    board_after_move(&g->list->board, side, m, e);
}

void game_list_destroy(BoardList *list)
{
    if (list == NULL)
        return;
    game_list_destroy(list->former);
    free(list);
}

void game_destroy(Game *g)
{
    game_list_destroy(g->list);
}