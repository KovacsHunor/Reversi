#ifndef IMAGE_H
#define IMAGE_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "master.h"


typedef struct Image{
    int x, y, w, h;
    SDL_Texture *sprite;
    bool visible;
}Image;

bool img_hover(Image* img, int x, int y);

Image img_init(int x, int y, char* path, Master* m, bool visible);

void img_render(Image* img, Master* m);


#endif