#ifndef IMAGE_H
#define IMAGE_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "master.h"
#include "debugmalloc.h"


/// @brief kép típus
typedef struct Image{
    pos p;
    pos size;
    SDL_Texture *sprite;
    bool visible;
}Image;

/// @brief megadja, hogy az adott pozíció a képen van-e
/// @param img a kép
/// @param p a pozíció
/// @return IGAZ ha a pozíció a képen van
bool img_hover(Image* img, pos p);

/// @brief visszatér egy kép típussal, a képet a path által meghatározott helyről olvassa be
/// @param p a kép poziíciója
/// @param path a kép fájl elérési útja
/// @param renderer a renderer
/// @param visible megadja, hogy látható-e kezdetben a kép
/// @return a kép
Image img_init(pos p, char* path, SDL_Renderer* renderer, bool visible);

/// @brief rendereli a képet
/// @param img a kép
/// @param renderer a renderer 
void img_render(Image* img, SDL_Renderer* renderer);

#endif