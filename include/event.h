#ifndef EVENT_H
#define EVENT_H
#include "image.h"
#include "master.h"
#include "board.h"
#include "game.h"
#include "button.h"
#include "gamelist.h"

void event_pressed_confirm(Controls *c, bool* ask, Game *g, button_id id, GameList **list);

void event_new(Game *g);

void event_load(Game *g);

void event_cont(Game *g, GameList **list);

void event_remove(GameList **list);

void event_basic(Controls *c, GameList **list, Game *g, Master *m);

void event_opponent(Controls *c, GameList **list, Game *g);

void event_color(Controls *c, GameList **list, Game *g);

void event_game(Controls *c, GameList **list, Game *g, Master *m);

void event_history(Controls *c, GameList **list, Game *g, Master *m);

void event_main(Controls *c, GameList **list, Game *g, Master *m);

#endif