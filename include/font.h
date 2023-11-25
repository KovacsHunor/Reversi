#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "master.h"
#include "utility.h"
#include "debugmalloc.h"

void font_render(SDL_Renderer *renderer, pos p, char *text);

#endif
