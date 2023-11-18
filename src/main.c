#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "debugmalloc.h"
#include "image.h"
#include "master.h"
#include "board.h"
#include "game.h"
#include "button.h"

#define BOARDSIZE 8

Uint32 timer(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

int main()
{
    Master master = master_init(1920, 1080, SDL_WINDOW_RESIZABLE); // option: SDL_WINDOW_MAXIMIZED
    TTF_Init();

    SDL_TimerID id = SDL_AddTimer(50, timer, NULL);

    SDL_SetRenderDrawColor(master.renderer, 0, 0, 0, 255);
    SDL_RenderClear(master.renderer);

    Game game = game_init(BLACK, AI);
    game_add_position(&game, &master);
    board_default(&game.list->board, &master);
    board_set_valid(&game.list->board, BLACK, &master);

    Controls controls;
    button_ctrl_init(&controls, &master);
    button_render_all(controls.arr, &master);

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

            button_render_all(controls.arr, &master);
            if (controls.arr[PLAY].pressed)
                board_render(&master, &game.history_board->board, side, e);
            if (controls.arr[BW].pressed)
            {
                if (game.history_board->former != NULL)
                {
                    game.history_board = game.history_board->former;
                    master.state = PREV;
                }
                controls.arr[BW].pressed = false;
            }
            if (controls.arr[FW].pressed)
            {
                
                if (game.history_board->next != NULL)
                {
                    game.history_board = game.history_board->next;
                }
                if (game.history_board->next == NULL) //looks bad, pretty sure can be done better
                {
                    master.state = GAME; // later -> function for continuing from position
                }
                controls.arr[FW].pressed = false;
            }
            SDL_RenderPresent(master.renderer);

            if (controls.arr[PLAY].pressed && side == WHITE && master.state == GAME)
            {
                game_AI_event(&game, &side, &master, &e);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                button_event(event.button.x, event.button.y, controls.arr, &master);
                if (controls.arr[PLAY].pressed && side == BLACK && master.state == GAME)
                {
                    game_player_event(&game, event.button.x, event.button.y, &side, &master, &e);
                }
            }
            break;
        case SDL_QUIT:
            quit = true;
            break;
        }
    }
    game_destroy(&game);
    ctrl_destroy(controls);
    SDL_RemoveTimer(id);
    SDL_Quit();
    return 0;
}