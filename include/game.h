#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <time.h>
#include "debugmalloc.h"

typedef struct BoardList{
    Board board;
    struct BoardList* former;
    struct BoardList* next;
}BoardList;

typedef enum Opponent{
    HUMAN, AI
}Opponent;

typedef enum Game_state{
    MATCH, OPPONENT, COLOR, PREV
}Game_state;

typedef struct Game{
    time_t date;    //epoch
    BoardList* list;
    Disk player_color;
    Opponent opponent;
    BoardList* history_board;
    Game_state state;
}Game;

void game_init(Game* g);

//-
bool game_player_event(Game* g, int x, int y);

//-
void game_AI_event(Game* g);

void game_add_position(Game* g);

void game_listcpy(BoardList** dst, BoardList* src);

void game_tofirst(BoardList **list);

void game_hbcpy(Game* dst, Game* src);

void game_cpy(Game* dst, Game* src);

void game_list_bwdestroy(BoardList* list);

void game_list_fwdestroy(BoardList* list);

void game_destroy(Game* g);

#endif