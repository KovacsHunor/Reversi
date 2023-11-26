#ifndef GAMELIST_H
#define GAMELIST_H

#include "game.h"
#include "debugmalloc.h"

/// @brief egyes meccseket tároló láncolt lista
typedef struct GameList{
    /// @brief adott meccs
    Game* game;
    struct GameList* former;
    struct GameList* next;
}GameList;

/// @brief hozzáfűz egy meccset a listához
/// @param list a lista
/// @param g a hozzáfűzendő elem
void gamelist_add(GameList** list, Game* g);

/// @brief felszabadítja a listát
/// @param list a lista
void gamelist_destroy(GameList* list);

/// @brief eltávolít egy elemet a listából
/// @param list_item az eltávolítandó elem
void gamelist_remove(GameList** list_item);

/// @brief a végére lépteti a listát
/// @param list a lista
void gamelist_tolast(GameList** list);

/// @brief betölti a listaelem adatait a cél meccsbe
/// @param g a cél meccs
/// @param list a listaelem
void gamelist_load(Game *g, GameList **list);

/// @brief a lista végére fűzi, majd elmenti a meccset
/// @param list a lista
/// @param g a meccs
void gamelist_save(GameList **list, Game *g);


/// @brief feltölti a listát a mentési fájlból származó adatokkal, a lefoglalt memória felszabadítása a hívó felelőssége
/// @param list a lista
void gamelist_fread(GameList **list);

/// @brief elmenti a lista adatait a mentési fájlba
/// @param list a lista
void gamelist_fprint(GameList **list);

#endif