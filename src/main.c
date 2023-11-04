#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "debugmalloc.h"
#include "image.h"
#include "master.h"
#include "board.h"
#include "quality_of_life.h"

#define BOARDSIZE 8

int main()
{
    Master master = master_init(1920, 1080, SDL_WINDOW_RESIZABLE); //option: SDL_WINDOW_MAXIMIZED
    TTF_Init();

    SDL_SetRenderDrawColor(master.renderer, 0, 0, 0, 255);
    SDL_RenderClear(master.renderer);

    Image menu = img_init(master.width-200, 100, "../sprites/menu_B.png", &master);
    img_draw(&master, &menu);

    Board board = board_init(800, &master);

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
                if (img_hover(&menu, event.button.x, event.button.y)){
                    state = GAME;
                    SDL_RenderClear(master.renderer);
                    board_draw(&master, &board);
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