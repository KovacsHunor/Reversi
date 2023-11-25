#ifndef FFONT_H
#define FFONT_H

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "master.h"
#include "utility.h"
#include "debugmalloc.h"

/// @brief rendereli a paraméterként kapott szöveget
/// @param renderer renderer 
/// @param p a szöveg helye
/// @param text a szöveg
void font_render(SDL_Renderer *renderer, pos p, char *text);

#endif
