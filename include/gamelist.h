#ifndef GAMELIST_H
#define GAMELIST_H

#include "game.h"
#include "debugmalloc.h"

/// @brief egyes meccseket tároló láncolt lista
typedef struct GameList{
    Game* game;
    struct GameList* former;
    struct GameList* next;
}GameList;

/// @brief hozzáfűz egy meccset a listához
/// @param list a lista
/// @param g a hozzáfűzendő elem
void gamelist_add(GameList** list, Game* g);

void gamelist_destroy(GameList* list);

void gamelist_remove(GameList** list_item);

void gamelist_tofirst(GameList** list);

void gamelist_load(Game *g, GameList **list);

void gamelist_save(GameList **list, Game *g, Master *m);

//-
void gamelist_fread(GameList **list, Master *m);

void gamelist_fprint(GameList **list);

#endif