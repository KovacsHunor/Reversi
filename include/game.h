#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <time.h>
#include "debugmalloc.h"

/// @brief játékállások listája
typedef struct BoardList{
    Board board;
    struct BoardList* former;
    struct BoardList* next;
}BoardList;


/// @brief az ellenfél típusa
typedef enum Opponent{
    HUMAN, AI
}Opponent;

/// @brief a játék állapota
typedef enum Game_state{
    MATCH, OPPONENT, COLOR, PREV
}Game_state;

/// @brief egy meccs adatai
typedef struct Game{
    time_t date;    //epoch
    BoardList* list;
    Disk player_color;
    Opponent opponent;
    BoardList* history_board;
    Game_state state;
}Game;

/// @brief inicializál egy meccset
/// @param g a meccs
void game_init(Game* g);

/// @brief megszünteti a meccs végét a history_board jelenlegi pozíciója után, így az lesz az új vége
/// @param g a meccs
void game_cut(Game *g);


/// @brief a felhasználó által megjelölt pozícióra rak egy korongot, és ennek megfelelően frissít a meccs adatain
/// @param g a meccs
/// @param p a megjelölt pozíció
void game_player_event(Game* g, pos p);

/// @brief választat egy pozíciót a géppel és rak oda egy korongot
/// @param g a meccs
void game_AI_event(Game* g);

/// @brief hozzáfűz egy új állást az eddigi álláslistához 
/// @param g 
static void game_add_position(Game* g);

/// @brief átmásolja dst-be az src által mutatott álláslistát, az állások felszabadítása a hívó felelőssége
/// @param dst ahova másol
/// @param src ahonnan másol
static void game_listcpy(BoardList** dst, BoardList* src);

/// @brief az utolsó állásba lépteti a listát
/// @param list a lista
void game_tolast(BoardList **list);

/// @brief beállítja a history_board-ot az cél meccsen a forrás mintájára
/// @param dst cél meccs
/// @param src forrás meccs
static void game_hbcpy(Game* dst, Game* src);

/// @brief átmásolja a forrás meccs adatait a cél meccs-be
/// @param dst cél meccs
/// @param src forrás meccs
void game_cpy(Game* dst, Game* src);

/// @brief felszabadítja a lístát a régebbi lépések irányában
/// @param list a lista
void game_list_bwdestroy(BoardList* list);

/// @brief felszabadítja a lístát az újabb lépések irányában
/// @param list a lista
void game_list_fwdestroy(BoardList* list);

/// @brief felszabadítja a meccshez tartozó lefoglalt területeket
/// @param g a meccs
void game_destroy(Game* g);

#endif