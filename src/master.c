#include "master.h"

Master master_init(int width, int height, Uint32 mode){
    Master m = {.width = width, .height = height, .mode = mode};
    master_set_SDL(&m);
    return m;
}

/*initialize sdl
master init uses it, do not call it elsewhere*/
void master_set_SDL(Master* m)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }

    if (m->mode == SDL_WINDOW_MAXIMIZED){
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        m->width = DM.w; 
        m->height = DM.h;
    }

    m -> window = SDL_CreateWindow("Reversi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m->width, m->height, m->mode);

    if (m->window == NULL)
    {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    m->renderer = SDL_CreateRenderer(m->window, -1, SDL_RENDERER_SOFTWARE);
    if (m->renderer == NULL)
    {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(m->renderer);
}
