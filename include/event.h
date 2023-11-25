#ifndef EVENT_H
#define EVENT_H
#include "image.h"
#include "master.h"
#include "board.h"
#include "game.h"
#include "button.h"
#include "gamelist.h"

static void event_pressed_confirm(Controls *c, bool* ask, Game *g, button_id id, GameList **list);

static void event_new(Game *g);

static void event_load(Game *g);

static void event_cont(Game *g, GameList **list);

static void event_remove(GameList **list);

static void event_basic(Controls *c, GameList **list, Game *g, Master *m);

static void event_opponent(Controls *c, GameList **list, Game *g);

static void event_color(Controls *c, GameList **list, Game *g);

static void event_game(Controls *c, GameList **list, Game *g, Master *m);

static void event_history(Controls *c, GameList **list, Game *g, Master *m);

static void event_askflip(Controls *c, bool *ask);

static void event_history_off(Controls *c);

void event_main(Controls *c, GameList **list, Game *g, Master *m);

#endif