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
#include "gamelist.h"
#include "event.h"

Uint32 timer(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

int main()
{
    Master master = master_init(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE); // option: SDL_WINDOW_MAXIMIZED
    TTF_Init();

    SDL_TimerID id = SDL_AddTimer(100, timer, NULL);

    SDL_SetRenderDrawColor(master.renderer, 0, 0, 0, 255);
    SDL_RenderClear(master.renderer);

    GameList *list = NULL;
    gamelist_load(&list, &master);

    Game game;
    game_init(&game);

    Controls controls;
    button_ctrl_init(&controls, master.renderer);
    button_render_all(controls.arr, master.renderer);

    SDL_RenderPresent(master.renderer);
    bool draw = false;
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_USEREVENT:
            SDL_RenderClear(master.renderer);
            event_main(&controls, &list, &game, &master);

            if (master.state == HISTORY)
            {
                if (list != NULL)
                    font_render(master.renderer, (pos){700, 500}, ctime(&list->game->date)); // probably allocates memory
                else
                    font_render(master.renderer, (pos){700, 500}, "nincs mentett játszma");
            }
            if (controls.arr[PLAY].pressed)
                board_render(master.renderer, &game.history_board->board);
            button_render_all(controls.arr, master.renderer);

            SDL_RenderPresent(master.renderer);

            if (game.state == MATCH && game.list->board.side == board_flip_color(game.player_color) && game.opponent == AI && game.list->board.state != END && master.state == GAME)
            {
                game_AI_event(&game);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                button_event(event.button.x, event.button.y, controls.arr, &master);
                button_render_all(controls.arr, master.renderer);
                if (!master.ask && game.state == MATCH && master.state == GAME && ((game.opponent == AI && game.list->board.side == game.player_color) || game.opponent == HUMAN))
                {
                    game_player_event(&game, event.button.x, event.button.y);
                }
            }
            break;
        case SDL_QUIT:
            quit = true;
            break;
        }
    }
    gamelist_destroy(list);
    game_list_bwdestroy(game.list);
    ctrl_destroy(controls);
    SDL_RemoveTimer(id);
    SDL_Quit();
    return 0;
}
