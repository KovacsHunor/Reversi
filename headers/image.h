#ifndef IMAGE_H
#define IMAGE_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>


typedef struct Image{
    int x, y, w, h;
    SDL_Texture *sprite;
    bool pressed;
}Image;

bool img_hover(Image img, int x, int y);

void img_init(Image* img, int x, int y, char* path, SDL_Renderer *renderer);

void img_draw(SDL_Renderer *renderer, Image img);


#endif