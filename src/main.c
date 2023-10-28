#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

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

void drawMenu(SDL_Renderer *renderer)
{
    SDL_Rect r = {WIDTH / 2 - 100, HEIGHT / 2 - 300, 200, 100};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer);
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

int main()
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(WIDTH, HEIGHT, &window, &renderer);

    enum State
    {
        QUIT,
        GAME,
        MENU
    };

    enum State state = MENU;
    drawMenu(renderer);
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
                if (event.button.x)
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