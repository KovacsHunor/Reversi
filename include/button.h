#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include "image.h"
#include "board.h"
#include "master.h"

typedef enum button_id
{
    MENU,
    PLAY,
    HISTORY,
    SIZE
}button_id;

typedef struct Button{
    Image img;
    SDL_Texture *sw_sprite;
    bool pressed;
}Button;

bool button_pressable(Button* b, button_id id, int x, int y);

void button_ctrl_default(Button* b);

void button_press(Button* b, button_id id);

void button_ctrl_init(Button* b, Master* m);

void button_event(int x, int y, Button* b, Master* m, Board* board);

void button_render_all(Button* b, Master* m);

#endif