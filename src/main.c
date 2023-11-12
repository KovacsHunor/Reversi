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

Uint32 idozit(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

int main()
{
    Master master = master_init(1920, 1080, SDL_WINDOW_RESIZABLE); //option: SDL_WINDOW_MAXIMIZED
    TTF_Init();

    SDL_TimerID id = SDL_AddTimer(50, idozit, NULL);

    SDL_SetRenderDrawColor(master.renderer, 0, 0, 0, 255);
    SDL_RenderClear(master.renderer);

    Board board = board_init(800, &master);
    board_default(&board, &master);
    board_set_valid(&board, BLACK, &master);

    Button ctrl_buttons[SIZE];
    button_ctrl_init(ctrl_buttons, &master);
    button_render_all(ctrl_buttons, &master);
    
    SDL_RenderPresent(master.renderer);

    b_event e = BASIC;
    disk_color side = BLACK;
    bool quit = false;
    while (!quit)
    {   
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_USEREVENT:
            SDL_RenderClear(master.renderer);

            button_render_all(ctrl_buttons, &master);
            if(ctrl_buttons[PLAY].pressed) board_render(&master, &board, side, e);

            SDL_RenderPresent(master.renderer);


            if(ctrl_buttons[PLAY].pressed && side == WHITE) board_AI_event(&board, &side, &master, &e);

            break;
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT){
                button_event(event.button.x, event.button.y, ctrl_buttons, &master);
                if(ctrl_buttons[PLAY].pressed && side == BLACK) board_player_event(&board, event.button.x, event.button.y, &side, &master, &e);
            }
            break;
        case SDL_QUIT:
            quit = true;
            break;
        }
    }

    SDL_RemoveTimer(id);
    SDL_Quit();
    return 0;
}