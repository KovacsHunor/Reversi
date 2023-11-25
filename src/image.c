#include "image.h"

bool img_hover(Image* img, int x, int y){
    return x <= img->x+img->w && x >= img->x && y <= img->y+img->h && y >= img->y;
}

Image img_init(int x, int y, char* path, SDL_Renderer* renderer, bool visible){
    Image img = (Image){x, y, 0, 0, IMG_LoadTexture(renderer, path), visible};
    if(img.sprite == NULL){
        printf("img loading error");
    }
    SDL_QueryTexture(img.sprite, NULL, NULL, &(img.w), &(img.h));
    return img;
}

void img_render(Image* img, SDL_Renderer* renderer)
{
    SDL_Rect rect;
    rect.x = img->x; rect.y = img->y; rect.w = img->w; rect.h = img->h; 
    SDL_RenderCopy(renderer, img->sprite, NULL, &rect);
}

void img_texture_swap(SDL_Texture** a, SDL_Texture** b){
    SDL_Texture* temp = *a;
    *a = *b;
    *b = temp;
}