#include "game.h"

void game_init(Game* g, disk_color player_c, Opponent opp, Master* m)
{
    *g = (Game){.date = time(NULL), .state = OPPONENT, .list = NULL, .player_color = player_c, .opponent = opp};
    game_add_position(g, m);
    board_default(&g->list->board, m);
    board_set_valid(&g->list->board, m);
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

        //g->list->board.state = BASIC;
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