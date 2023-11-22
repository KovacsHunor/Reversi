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

void gamelist_new(GameList** listp, Master* m);

#endif