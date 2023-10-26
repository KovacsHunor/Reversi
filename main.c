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

enum State{QUIT, GAME, MENU};

void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("SDL peldaprogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
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

Uint32 idozit(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

void drawFont(SDL_Renderer *renderer, int x, int y, char* text){
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", 32);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };

    felirat = TTF_RenderUTF8_Solid(font, text, (SDL_Color){255, 255, 255});
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = x;
    hova.y = y;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}

void drawMenu(SDL_Renderer *renderer){
    drawFont(renderer, 500, 500, "Menu");
    SDL_RenderPresent(renderer);
}

void drawTable(SDL_Renderer *renderer)
{
    for (int i = 0; i < BOARDSIZE; i++)
    {
        for (int j = 0; j < BOARDSIZE; j++)
        {
            SDL_Rect r = {i*(TILESIZE+1) + 100, j*(TILESIZE+1) + 100, TILESIZE, TILESIZE};
            SDL_SetRenderDrawColor(renderer, 0, (i+j)%2?120:130, 0, 255);
            SDL_RenderFillRect(renderer, &r);
        }
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{

    int cx = 800;
    int cy = 400;
    int zoom = 1;

    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(WIDTH, HEIGHT, &window, &renderer);

    /* idozito hozzaadasa: 20 ms; 1000 ms / 20 ms -> 50 fps */
    SDL_TimerID id = SDL_AddTimer(20, idozit, NULL);

    /* animaciohoz */
    /* szokasos esemenyhurok */
    
    enum State state = MENU;
    while(state != QUIT)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        /* felhasznaloi esemeny: ilyeneket general az idozito fuggveny */
        case SDL_USEREVENT:
            switch(state){
                case GAME:
                    drawTable(renderer);
                    break;
                case MENU:
                    drawMenu(renderer);
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
            cx = event.button.x;
            cy = event.button.y;
            break;
        case SDL_QUIT:
            state = QUIT;
            break;
        }
    }
    /* idozito torlese */
    SDL_RemoveTimer(id);

    SDL_Quit();

    return 0;
}