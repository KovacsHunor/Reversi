#include "image.h"
#include "master.h"

bool img_hover(Image* img, int x, int y){
    return x <= img->x+img->w && x >= img->x && y <= img->y+img->h && y >= img->y;
}

Image img_init(int x, int y, char* path, Master* m){
    Image img = (Image){x, y, 0, 0, IMG_LoadTexture(m->renderer, path), false};
    SDL_QueryTexture(img.sprite, NULL, NULL, &(img.w), &(img.h));
    return img;
}

void img_draw(Master* m, Image* img)
{
    SDL_Rect rect;
    rect.x = img->x; rect.y = img->y; rect.w = img->w/2; rect.h = img->h/2; 
    SDL_RenderCopy(m->renderer, img->sprite, NULL, &rect);
    SDL_RenderPresent(m->renderer);
}