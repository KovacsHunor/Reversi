#ifndef GAME_H
#define GAME_H

#include "board.h"

typedef struct BoardList{
    Board board;
    struct BoardList* former;
    struct BoardList* next;
}BoardList;

typedef enum Opponent{
    HUMAN, AI
}Opponent;

typedef struct Game{
    BoardList* list;
    disk_color player_color;
    Opponent opponent;
    BoardList* history_board;
}Game;

Game game_init(disk_color player_c, Opponent opp);

//-
void game_player_event(Game* g, int x, int y, disk_color* side, Master* m, b_event *e);

//-
void game_AI_event(Game* g, disk_color* side, Master* m, b_event *e);


void game_add_position(Game* g, Master* m);

void game_list_destroy(BoardList* list);

void game_destroy(Game* g);

#endif