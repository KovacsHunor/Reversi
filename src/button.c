#include "button.h"

void button_ctrl_init(Button *arr, SDL_Renderer* renderer)
{
    arr[MENU] = (Button){img_init(WIDTH - 200, 100, "../sprites/menu_B.png",renderer, true), false};
    arr[PLAY] = (Button){img_init(WIDTH - 200, 300, "../sprites/play_B.png",renderer, true), false};
    arr[B_HISTORY] = (Button){img_init(WIDTH - 200, 500, "../sprites/history_B.png",renderer, true), false};
    arr[PREV_BW] = (Button){img_init(WIDTH - 400,HEIGHT - 300, "../sprites/arrow_L.png",renderer, false), false};
    arr[PREV_FW] = (Button){img_init(WIDTH - 200,HEIGHT - 300, "../sprites/arrow_R.png",renderer, false), false};
    arr[PERSON] = (Button){img_init(840, 330, "../sprites/person.png",renderer, false), false};
    arr[ROBOT] = (Button){img_init(840, 530, "../sprites/robot.png",renderer, false), false};
    arr[B_BLACK] = (Button){img_init(850, 350, "../sprites/disk_B.png",renderer, false), false};
    arr[B_WHITE] = (Button){img_init(850, 550, "../sprites/disk_W.png",renderer, false), false};
    arr[B_NEW] = (Button){img_init(200,HEIGHT - 150, "../sprites/new.png",renderer, false), false};
    arr[HISTORY_FW] = (Button){img_init(600,HEIGHT - 300, "../sprites/arrow_L.png",renderer, false), false};
    arr[HISTORY_BW] = (Button){img_init(WIDTH - 700,HEIGHT - 300, "../sprites/arrow_R.png",renderer, false), false};
    arr[CUT] = (Button){img_init(200,HEIGHT - 500, "../sprites/load.png",renderer, false), false};
    arr[LOAD] = (Button){img_init(800,HEIGHT - 300, "../sprites/load.png",renderer, false), false};
    arr[DELETE] = (Button){img_init(1000,HEIGHT - 300, "../sprites/delete.png",renderer, false), false};
    arr[SAVE] = (Button){img_init(200,HEIGHT - 320, "../sprites/save.png",renderer, false), false};
    arr[YES] = (Button){img_init(100, 200, "../sprites/yes.png",renderer, false), false};
    arr[NO] = (Button){img_init(300, 200, "../sprites/delete.png",renderer, false), false};
}

void button_ctrl_minimal(Button *b)
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

bool button_pressable(Button *b, button_id id, pos p)
{
    return b[id].img.visible && !b[id].pressed && img_hover(&b[id].img, p.x, p.y);
}

void button_ctrl_destroy(Button* b)
{
    for (int i = 0; i < SIZE; i++)
    {
        SDL_DestroyTexture(b[i].img.sprite);
    }
}

void button_event(pos p, Button *b, Master *m)
{
    if (img_hover(&b[MENU].img, p.x, p.y))
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
            if (button_pressable(b, i, p))
            {
                if (i == PLAY || i == B_HISTORY)
                {
                    button_ctrl_minimal(b);
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
        if (button_pressable(b, YES, p)) b[YES].pressed = true;
        if (button_pressable(b, NO, p)) b[NO].pressed = true;
    }
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