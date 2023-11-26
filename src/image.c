#include "image.h"

bool img_hover(Image* img, pos p){
    return pos_hover(img->p, img->size, p);
}

Image img_init(pos p, char* path, SDL_Renderer* renderer, bool visible){
    Image img = (Image){p, (pos){0, 0}, IMG_LoadTexture(renderer, path), visible};
    if(img.sprite == NULL){
        printf("img loading error");
    }
    SDL_QueryTexture(img.sprite, NULL, NULL, &(img.size.x), &(img.size.y));
    return img;
}

void img_render(Image* img, SDL_Renderer* renderer)
{
    SDL_Rect rect;
    rect.x = img->p.x; rect.y = img->p.y; rect.w = img->size.x; rect.h = img->size.y; 
    SDL_RenderCopy(renderer, img->sprite, NULL, &rect);
}