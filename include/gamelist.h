#ifndef GAMELIST_H
#define GAMELIST_H

#include "game.h"

typedef struct GameList{
    Game* game;
    struct GameList* former;
    struct GameList* next;
}GameList;

void gamelist_add(GameList** list, Game* g);

void gamelist_destroy(GameList* list);

void gamelist_remove(GameList** list_item);

void gamelist_tofirst(GameList** list);

void gamelist_save(GameList **list, Game *g, Master *m);

void gamelist_load(GameList **list, Master *m);

void gamelist_fprint(GameList **list);

#endif