#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

enum
{
    BOARDSIZE = 8,
    TILESIZE = 100,
    HEIGHT = 1000,
    WIDTH = 1000
};

/*
initialize sdl
puts the size of the window in width and height parameters*/
void sdl_init(int* width, int* height, SDL_Window **pwindow, SDL_Renderer **prenderer, Uint32 mode)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("SDL peldaprogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *width, *height, mode);
    SDL_GetWindowSize(window, width, height);
    if (window == NULL)
    {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

void drawTable(SDL_Renderer *renderer)
{
    for (int i = 0; i < BOARDSIZE; i++)
    {
        for (int j = 0; j < BOARDSIZE; j++)
        {
            SDL_Rect r = {i * (TILESIZE + 1) + 100, j * (TILESIZE + 1) + 100, TILESIZE, TILESIZE};
            SDL_SetRenderDrawColor(renderer, 0, (i + j) % 2 ? 120 : 130, 0, 255);
            SDL_RenderFillRect(renderer, &r);
        }
    }
    SDL_RenderPresent(renderer);
}

typedef struct Image{
    int x, y, w, h;
    SDL_Texture *sprite;
    bool pressed;
}Image;

bool img_hover(Image img, int x, int y){
    return x <= img.x+img.w && x >= img.x && y <= img.y+img.h && y >= img.y;
}

void renderText(SDL_Renderer *renderer, int x, int y, int w, int h, int pt, SDL_Color c, char* text){
    TTF_Font *font = TTF_OpenFont("../fonts/LiberationSerif-Regular.ttf", pt);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };

    felirat = TTF_RenderUTF8_Solid(font, text, c);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = x;
    hova.y = y;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}

void setupImage(Image* img, int x, int y, char* path, SDL_Renderer *renderer){
    *img = (Image){x, y, 0, 0, IMG_LoadTexture(renderer, path), false};
    SDL_QueryTexture(img->sprite, NULL, NULL, &(img->w), &(img->h));
}

void drawImage(SDL_Renderer *renderer, Image img)
{
    SDL_Rect rect;
    rect.x = img.x; rect.y = img.y; rect.w = img.w*0.5; rect.h = img.h*0.5; 
    SDL_RenderCopy(renderer, img.sprite, NULL, &rect);
    SDL_RenderPresent(renderer);
}


int main()
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width = 1000, height = 1000;
    sdl_init(&width, &height, &window, &renderer, 0);
    TTF_Init();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Image menu;
    setupImage(&menu, width-150, 50, "../sprites/menu_B.png", renderer);

    drawImage(renderer, menu);

    enum State
    {
        NEW,
        GAME,
        SAVED,
        QUIT
    };

    enum State state = NEW;
    while (state != QUIT)
    {   
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT){
            switch (state)
            {
            case NEW:
                if (img_hover(menu, event.button.x, event.button.y)){
                    state = GAME;
                    SDL_RenderClear(renderer);
                    drawTable(renderer);
                }
                break;
            }
            }
            break;
        case SDL_QUIT:
            state = QUIT;
            break;
        }
    }

    SDL_Quit();
    return 0;
}