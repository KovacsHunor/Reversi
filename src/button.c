#include "button.h"

void button_ctrl_init(Controls *c, Master *m)
{
    c->arr[MENU] = (Button){img_init(m->width - 200, 100, "../sprites/menu_B.png", m, true), IMG_LoadTexture(m->renderer, "../sprites/menu_A.png"), false};
    c->arr[PLAY] = (Button){img_init(m->width - 200, 300, "../sprites/play_B.png", m, true), IMG_LoadTexture(m->renderer, "../sprites/play_A.png"), false};
    c->arr[B_HISTORY] = (Button){img_init(m->width - 200, 500, "../sprites/history_B.png", m, true), IMG_LoadTexture(m->renderer, "../sprites/history_A.png"), false};
    c->arr[BW] = (Button){img_init(m->width - 400, m->height - 300, "../sprites/arrow_L.png", m, false), IMG_LoadTexture(m->renderer, "../sprites/arrow_L.png"), false};
    c->arr[FW] = (Button){img_init(m->width - 200, m->height - 300, "../sprites/arrow_R.png", m, false), IMG_LoadTexture(m->renderer, "../sprites/arrow_R.png"), false};
    c->arr[PERSON] = (Button){img_init(800, 200, "../sprites/person.png", m, false), IMG_LoadTexture(m->renderer, "../sprites/person.png"), false};
    c->arr[ROBOT] = (Button){img_init(1000, 200, "../sprites/robot.png", m, false), IMG_LoadTexture(m->renderer, "../sprites/robot.png"), false};
    c->arr[B_BLACK] = (Button){img_init(800, 200, "../sprites/disk_B.png", m, false), IMG_LoadTexture(m->renderer, "../sprites/disk_B.png"), false};
    c->arr[B_WHITE] = (Button){img_init(1000, 200, "../sprites/disk_W.png", m, false), IMG_LoadTexture(m->renderer, "../sprites/disk_W.png"), false};
    c->arr[B_NEW] = (Button){img_init(200, 900, "../sprites/new.png", m, false), IMG_LoadTexture(m->renderer, "../sprites/new.png"), false};
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

bool button_tasks(Controls *c, GameList **listp, Master *m)
{
    GameList *list = *listp;
    if (m->state == GAME)
    {
        if (list->game->state == MATCH || list->game->state == PREV)
        {
            c->arr[BW].img.visible = true;
            c->arr[FW].img.visible = true;
            c->arr[B_NEW].img.visible = true;
            if (c->arr[BW].pressed)
            {
                if (list->game->history_board->former != NULL)
                {
                    list->game->history_board = list->game->history_board->former;
                    list->game->state = PREV;
                }
                c->arr[BW].pressed = false;
            }
            else if (c->arr[FW].pressed)
            {
                if (list->game->history_board->next != NULL)
                {
                    list->game->history_board = list->game->history_board->next;
                }
                if (list->game->history_board->next == NULL) // looks bad, pretty sure can be done better
                {
                    list->game->state = MATCH; // later -> function for continuing from position
                }
                c->arr[FW].pressed = false;
            }
            else if (c->arr[B_NEW].pressed)
            {
                gamelist_new(listp, m);
                (*listp)->game->state = OPPONENT;
                c->arr[B_NEW].pressed = false;
                return true;
            }
        }
        else
        {
            c->arr[FW].img.visible = false;
            c->arr[BW].img.visible = false;
            c->arr[B_NEW].img.visible = false;
        }
        if (list->game->state == OPPONENT)
        {
            c->arr[PERSON].img.visible = true;
            c->arr[ROBOT].img.visible = true;
            if (c->arr[PERSON].pressed)
            {
                list->game->opponent = HUMAN;
                list->game->state = MATCH;
                c->arr[PERSON].pressed = false;
            }
            else if (c->arr[ROBOT].pressed)
            {
                list->game->opponent = AI;
                list->game->state = COLOR;
                c->arr[ROBOT].pressed = false;
            }
            else
                return true;
            c->arr[PERSON].img.visible = false;
            c->arr[ROBOT].img.visible = false;

            return true;
        }
        if (list->game->state == COLOR)
        {
            c->arr[B_BLACK].img.visible = true;
            c->arr[B_WHITE].img.visible = true;
            if (c->arr[B_BLACK].pressed)
            {
                list->game->player_color = BLACK;
                c->arr[B_BLACK].pressed = false;
            }

            else if (c->arr[B_WHITE].pressed)
            {
                list->game->player_color = WHITE;
                c->arr[B_WHITE].pressed = false;
            }

            else
                return true;
            list->game->state = MATCH;
            c->arr[B_BLACK].img.visible = false;
            c->arr[B_WHITE].img.visible = false;

            return true;
        }
    }
    return false;
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
        img_texture_swap(&b[MENU].sw_sprite, &b[MENU].img.sprite);
        b[MENU].pressed = !b[MENU].pressed;
    }
    for (int i = 1; i < SIZE; i++)
    {
        if (button_pressable(b, i, x, y))
        {
            if (i == PLAY || i == B_HISTORY)
            {
                button_ctrl_default(b);
                button_press(b, i);
                if (i == PLAY)
                    m->state = GAME;
                if (i == B_HISTORY)
                    m->state = HISTORY;
            }
            else
            {
                b[i].pressed = true;
            }
        }
    }
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