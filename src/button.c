#include "button.h"

void button_ctrl_init(Controls *c, Master *m)
{
    c->arr[MENU] = (Button){img_init(m->width - 200, 100, "../sprites/menu_B.png", m, true), IMG_LoadTexture(m->renderer, "../sprites/menu_A.png"), false};
    c->arr[PLAY] = (Button){img_init(m->width - 200, 300, "../sprites/play_B.png", m, true), IMG_LoadTexture(m->renderer, "../sprites/play_A.png"), false};
    c->arr[B_HISTORY] = (Button){img_init(m->width - 200, 500, "../sprites/history_B.png", m, true), IMG_LoadTexture(m->renderer, "../sprites/history_A.png"), false};
    c->arr[BW] = (Button){img_init(m->width - 400, m->height - 300, "../sprites/arrow_L.png", m, false), IMG_LoadTexture(m->renderer, "../sprites/history_A.png"), false};
    c->arr[FW] = (Button){img_init(m->width - 200, m->height - 300, "../sprites/arrow_R.png", m, false), IMG_LoadTexture(m->renderer, "../sprites/history_A.png"), false};
    c->size = SIZE;
}

void button_press(Button *b, button_id id)
{
    img_texture_swap(&b[id].sw_sprite, &b[id].img.sprite);
    b[id].pressed = true;
}

void button_ctrl_default(Button *b)
{
    for (int i = 1; i < SIZE; i++)
    {
        if (b[i].pressed)
        {
            img_texture_swap(&b[i].sw_sprite, &b[i].img.sprite);
            b[i].pressed = false;
        }
        if (i == FW || i == BW)
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

void button_tasks(Controls *c, Game *g, Master *m)
{
    if (c->arr[BW].pressed)
    {
        if (g->history_board->former != NULL)
        {
            g->history_board = g->history_board->former;
            m->state = PREV;
        }
        c->arr[BW].pressed = false;
    }
    if (c->arr[FW].pressed)
    {
        if (g->history_board->next != NULL)
        {
            g->history_board = g->history_board->next;
        }
        if (g->history_board->next == NULL) // looks bad, pretty sure can be done better
        {
            m->state = GAME; // later -> function for continuing from position
        }
        c->arr[FW].pressed = false;
    }
}

bool button_event(int x, int y, Button *b, Master *m)
{
    if (img_hover(&b[MENU].img, x, y))
    {

        for (int i = 1; i < SIZE; i++)
        {
            if (i != FW && i != BW) b[i].img.visible = !b[i].img.visible;
        }
        img_texture_swap(&b[MENU].sw_sprite, &b[MENU].img.sprite);
        b[MENU].pressed = !b[MENU].pressed;
    }
    else if (img_hover(&b[BW].img, x, y))
    {
        b[BW].pressed = true;
    }
    else if (img_hover(&b[FW].img, x, y))
    {
        b[FW].pressed = true;
    }
    else if (button_pressable(b, PLAY, x, y))
    {
        button_ctrl_default(b);
        b[BW].img.visible = true;
        b[FW].img.visible = true;
        button_press(b, PLAY);
    }
    else if (button_pressable(b, B_HISTORY, x, y))
    {
        button_ctrl_default(b);
        button_press(b, B_HISTORY);
    }
    else    return false;
    button_render_all(b, m);
    return true;
}

void button_render_all(Button *b, Master *m)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (b[i].img.visible)
        {
            img_render(&b[i].img, m);
        }
    }
}