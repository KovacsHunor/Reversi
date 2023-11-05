#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "debugmalloc.h"
#include "image.h"
#include "master.h"
#include "board.h"
#include "button.h"

#define BOARDSIZE 8

int main()
{
    Master master = master_init(1920, 1080, SDL_WINDOW_RESIZABLE); //option: SDL_WINDOW_MAXIMIZED
    TTF_Init();

    SDL_SetRenderDrawColor(master.renderer, 0, 0, 0, 255);
    SDL_RenderClear(master.renderer);

    Board board = board_init(800, &master);

    Button ctrl_buttons[SIZE];
    button_ctrl_init(ctrl_buttons, &master);
    button_render_all(ctrl_buttons, &master);


    bool quit = false;
    while (!quit)
    {   
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT) button_event(event.button.x, event.button.y, ctrl_buttons, &master, &board);
            break;
        case SDL_QUIT:
            quit = true;
            break;
        }
    }

    SDL_Quit();
    return 0;
}