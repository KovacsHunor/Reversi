#ifndef GAME_H
#define GAME_H

#include <unistd.h>
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

void game_init(Game* g, disk_color player_c, Opponent opp, Master* m);

//-
bool game_player_event(Game* g, int x, int y, Master* m);

//-
void game_AI_event(Game* g, Master* m);

void game_add_position(Game* g, Master* m);

void game_list_destroy(BoardList* list);

void game_destroy(Game* g);

#endif