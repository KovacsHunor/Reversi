#ifndef EVENT_H
#define EVENT_H
#include "image.h"
#include "master.h"
#include "board.h"
#include "game.h"
#include "button.h"
#include "gamelist.h"

/// @brief adatvesztés lehetőségével járó gombnyomások megerősítésének kezelése
/// @param controls gombok tömbje
/// @param ask a megerősítőfelület állapota
/// @param g az adott meccs
/// @param id a gomb azonosítója
/// @param list a meccsek listája
void event_pressed_confirm(Button *controls, bool* ask, Game *g, Button_id id, GameList **list);

void event_basic(Button *controls, GameList **list, Game *g, Master *m);

void event_opponent(Button *controls, GameList **list, Game *g);

void event_color(Button *controls, GameList **list, Game *g);

void event_game(Button *controls, GameList **list, Game *g, Master *m);

void event_history(Button *controls, GameList **list, Game *g, bool* ask);

void event_askflip(Button *controls, bool *ask);

void event_history_off(Button *controls);


/// @brief a vezérőgombok állapota alapján állítja a program állapotát
/// @param controls a gombok tömbje
/// @param list a meccsek listája
/// @param g a jelenlegi meccs
/// @param m általános állapotokat tároló típus
void event_main(Button *controls, GameList **list, Game *g, Master *m);

#endif