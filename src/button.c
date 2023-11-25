#include "button.h"

void button_ctrl_init(Controls *c, SDL_Renderer* renderer)
{
    c->arr[MENU] = (Button){img_init(WIDTH - 200, 100, "../sprites/menu_B.png",renderer, true), false};
    c->arr[PLAY] = (Button){img_init(WIDTH - 200, 300, "../sprites/play_B.png",renderer, true), false};
    c->arr[B_HISTORY] = (Button){img_init(WIDTH - 200, 500, "../sprites/history_B.png",renderer, true), false};
    c->arr[PREV_BW] = (Button){img_init(WIDTH - 400,HEIGHT - 300, "../sprites/arrow_L.png",renderer, false), false};
    c->arr[PREV_FW] = (Button){img_init(WIDTH - 200,HEIGHT - 300, "../sprites/arrow_R.png",renderer, false), false};
    c->arr[PERSON] = (Button){img_init(840, 330, "../sprites/person.png",renderer, false), false};
    c->arr[ROBOT] = (Button){img_init(840, 530, "../sprites/robot.png",renderer, false), false};
    c->arr[B_BLACK] = (Button){img_init(850, 350, "../sprites/disk_B.png",renderer, false), false};
    c->arr[B_WHITE] = (Button){img_init(850, 550, "../sprites/disk_W.png",renderer, false), false};
    c->arr[B_NEW] = (Button){img_init(200,HEIGHT - 150, "../sprites/new.png",renderer, false), false};
    c->arr[HISTORY_FW] = (Button){img_init(600,HEIGHT - 300, "../sprites/arrow_L.png",renderer, false), false};
    c->arr[HISTORY_BW] = (Button){img_init(WIDTH - 700,HEIGHT - 300, "../sprites/arrow_R.png",renderer, false), false};
    c->arr[LOAD] = (Button){img_init(200,HEIGHT - 500, "../sprites/load.png",renderer, false), false};
    c->arr[CONT] = (Button){img_init(800,HEIGHT - 300, "../sprites/load.png",renderer, false), false};
    c->arr[DELETE] = (Button){img_init(1000,HEIGHT - 300, "../sprites/delete.png",renderer, false), false};
    c->arr[SAVE] = (Button){img_init(200,HEIGHT - 320, "../sprites/save.png",renderer, false), false};
    c->arr[YES] = (Button){img_init(100, 200, "../sprites/yes.png",renderer, false), false};
    c->arr[NO] = (Button){img_init(300, 200, "../sprites/delete.png",renderer, false), false};
    c->size = SIZE;
}

void button_ctrl_default(Button *b)
{
    for (int i = 1; i < SIZE; i++)
    {
        if (b[i].pressed)
        {
            b[i].pressed = false;
        }
        if (i != B_HISTORY && i != PLAY)
        {
            b[i].img.visible = false;
        }
    }
}

bool button_pressable(Button *b, button_id id, int x, int y)
{
    return b[id].img.visible && !b[id].pressed && img_hover(&b[id].img, x, y);
}

void ctrl_destroy(Controls c)
{
    for (int i = 0; i < c.size; i++)
    {
        SDL_DestroyTexture(c.arr[i].img.sprite);
    }
}

bool button_event(int x, int y, Button *b, Master *m)
{
    if (img_hover(&b[MENU].img, x, y))
    {
        for (int i = 1; i < SIZE; i++)
        {
            if (i == PLAY || i == B_HISTORY)
                b[i].img.visible = !b[i].img.visible;
        }
        b[MENU].pressed = !b[MENU].pressed;
    }
    if (!m->ask)
    {
        for (int i = 1; i < SIZE; i++)
        {
            if (button_pressable(b, i, x, y))
            {
                if (i == PLAY || i == B_HISTORY)
                {
                    button_ctrl_default(b);
                    if (i == PLAY)
                        m->state = GAME;
                    if (i == B_HISTORY)
                        m->state = HISTORY;
                }
                b[i].pressed = true;
            }
        }
    }
    else{
        if (button_pressable(b, YES, x, y)) b[YES].pressed = true;
        if (button_pressable(b, NO, x, y)) b[NO].pressed = true;
    }

    button_render_all(b, m->renderer);
    return true;
}

void button_render_all(Button *b, SDL_Renderer* renderer)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (b[i].img.visible)
        {
            img_render(&b[i].img, renderer);
        }
    }
}