#ifndef MASTER_H
#define MASTER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "utility.h"

/// @brief típus a játék állapotára
typedef enum State{
    GAME, HISTORY
}State;

/// @brief általános adatokat és állapotokat tartalmazó típus
typedef struct Master{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width;
    int height;
    Uint32 mode;
    bool ask;
    State state;
}Master;

/// @brief inicializál egy Master típust
/// @param mode az ablak megjelenítési módja
/// @return egy Mester típus
Master master_init(Uint32 mode);

/// @brief elvégzi a szükséges alapbeállításokat az sdl használatához
/// @param m mester típus, az ablak megjelenítési módjának függvényébe változhat a width és height értéke
void master_set_SDL(Master* m);

#endif