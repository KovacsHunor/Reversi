#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "image.h"
#include "master.h"
#include "debugmalloc.h"
#include "game.h"
#include "gamelist.h"

typedef enum button_id
{
    MENU,
    PLAY,
    B_HISTORY,
    PREV_FW,
    PREV_BW,
    PERSON,
    ROBOT,
    B_BLACK,
    B_WHITE,
    B_NEW,
    HISTORY_FW,
    HISTORY_BW,
    LOAD,
    CONT,
    DELETE,
    SAVE,
    YES,
    NO,
    SIZE
} button_id;

typedef struct Button
{
    Image img;
    bool pressed;
} Button;

typedef struct Controls
{
    Button arr[SIZE];
    int size;
} Controls;

void ctrl_destroy(Controls c);

bool button_pressable(Button *b, button_id id, int x, int y);

bool button_tasks(Controls *c, GameList **list, Game* g, GameList **mover, Master *m);

void button_ctrl_default(Button *b);

void button_ctrl_init(Controls *c, SDL_Renderer* renderer);

bool button_event(int x, int y, Button *b, Master* m);

//-
void button_render_all(Button *b, SDL_Renderer* renderer);

#endif