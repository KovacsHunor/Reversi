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
#include "ffont.h"

Uint32 timer(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

int main()
{
    Master master = master_init(SDL_WINDOW_RESIZABLE); // option: SDL_WINDOW_MAXIMIZED
    TTF_Init();

    SDL_TimerID id = SDL_AddTimer(100, timer, NULL);

    SDL_SetRenderDrawColor(master.renderer, 0, 0, 0, 255);
    SDL_RenderClear(master.renderer);

    GameList *list = NULL;
    gamelist_fread(&list);

    Game game;
    game_init(&game);

    Button controls[SIZE];
    button_ctrl_init(controls, master.renderer);
    button_render_all(controls, master.renderer);

    SDL_RenderPresent(master.renderer);
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_USEREVENT:
            SDL_RenderClear(master.renderer);
            event_main(controls, &list, &game, &master);

            if (master.ask)
                font_render(master.renderer, (pos){200, 100}, "biztos?");
            if (master.state == HISTORY)
                font_render(master.renderer, (pos){700, 500}, list != NULL?ctime(&list->game->date):"nincs mentett játszma");
            if (controls[PLAY].pressed)
                board_render(master.renderer, &game.list->board, game.player_color);
            
            button_render_all(controls, master.renderer);
            SDL_RenderPresent(master.renderer);

            if (game.state == MATCH && game.list->board.side == board_flip_color(game.player_color) && game.opponent == AI && game.list->board.state != END && master.state == GAME)
            {
                game_AI_event(&game);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                button_event((pos){event.button.x, event.button.y}, controls, &master);
                button_render_all(controls, master.renderer);
                if (!master.ask && game.state == MATCH && master.state == GAME && ((game.opponent == AI && game.list->board.side == game.player_color) || game.opponent == HUMAN))
                {
                    game_player_event(&game, (pos){event.button.x, event.button.y});
                }
            }
            break;
        case SDL_QUIT:
            quit = true;
            break;
        }
    }
    gamelist_tolast(&list);
    gamelist_destroy(list);
    game_tolast(&game.list);
    game_list_bwdestroy(game.list);
    button_ctrl_destroy(controls);
    SDL_RemoveTimer(id);
    SDL_DestroyRenderer(master.renderer);
    master.renderer = NULL;
    SDL_DestroyWindow(master.window);
    master.window = NULL;
    SDL_Quit();
    return 0;
}
