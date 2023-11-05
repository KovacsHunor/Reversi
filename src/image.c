#include "image.h"

bool img_hover(Image* img, int x, int y){
    return x <= img->x+img->w && x >= img->x && y <= img->y+img->h && y >= img->y;
}

Image img_init(int x, int y, char* path, Master* m, bool visible){
    Image img = (Image){x, y, 0, 0, IMG_LoadTexture(m->renderer, path), visible};
    SDL_QueryTexture(img.sprite, NULL, NULL, &(img.w), &(img.h));
    return img;
}

void img_draw(Image* img, Master* m)
{
    SDL_Rect rect;
    rect.x = img->x; rect.y = img->y; rect.w = img->w; rect.h = img->h; 
    SDL_RenderCopy(m->renderer, img->sprite, NULL, &rect);
    SDL_RenderPresent(m->renderer);
}