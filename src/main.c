#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

enum
{
    BOARDSIZE = 8,
    TILESIZE = 100,
    HEIGHT = 1000,
    WIDTH = 1000
};

void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("SDL peldaprogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, SDL_WINDOW_MAXIMIZED);
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

typedef struct Button{
    int x, y, w, h;
    char* text;
    SDL_Color bg_color, text_color;
}Button;

bool bt_hover(Button bt, int x, int y){
    return x <= bt.x+bt.w && x >= bt.x && y <= bt.y+bt.h && y >= bt.y;
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

void renderRect(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color c){
    SDL_Rect r = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer, &r);
}

void drawButton(SDL_Renderer *renderer, Button bt)
{
    int pt = 32;
    renderRect(renderer, bt.x, bt.y, bt.w, bt.h, bt.bg_color);
    renderText(renderer, bt.x, bt.y, bt.w, bt.h, pt, bt.text_color, bt.text);
    SDL_RenderPresent(renderer);
}





int main()
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(WIDTH, HEIGHT, &window, &renderer);
    TTF_Init();

    Button menu = {100, 100, 200, 50, "Menu", (SDL_Color){100, 100, 100}, (SDL_Color){255, 255, 255}};
    drawButton(renderer, menu);

    enum State
    {
        QUIT,
        GAME,
        MENU
    };

    enum State state = MENU;
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
            case MENU:
                if (bt_hover(menu, event.button.x, event.button.y)){
                    state = GAME;
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