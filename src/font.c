#include "font.h"
//#include "debugmalloc.h"



/// @brief renderel egy szöveget
/// @param renderer renderer 
/// @param p a szöveg helye
/// @param text a szöveg
void font_render(SDL_Renderer* renderer, pos p, char* text){
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("../fonts/LiberationSerif-Regular.ttf", 48);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };

    felirat = TTF_RenderUTF8_Solid(font, text, (SDL_Color){255, 255, 255});
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = p.x;
    hova.y = p.y;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    TTF_CloseFont(font);
}