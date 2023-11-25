#ifndef MASTER_H
#define MASTER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "utility.h"

typedef enum State{
    GAME, HISTORY
}State;

typedef struct Master{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width;
    int height;
    Uint32 mode;
    bool ask;
    State state;
}Master;

Master master_init(int width, int height, Uint32 mode);

void master_set_SDL(Master* m);

#endif