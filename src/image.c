#include "../headers/image.h"

bool img_hover(Image img, int x, int y){
    return x <= img.x+img.w && x >= img.x && y <= img.y+img.h && y >= img.y;
}

void img_init(Image* img, int x, int y, char* path, SDL_Renderer *renderer){
    *img = (Image){x, y, 0, 0, IMG_LoadTexture(renderer, path), false};
    SDL_QueryTexture(img->sprite, NULL, NULL, &(img->w), &(img->h));
}

void img_draw(SDL_Renderer *renderer, Image img)
{
    SDL_Rect rect;
    rect.x = img.x; rect.y = img.y; rect.w = img.w*0.5; rect.h = img.h*0.5; 
    SDL_RenderCopy(renderer, img.sprite, NULL, &rect);
    SDL_RenderPresent(renderer);
}