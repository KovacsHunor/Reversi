#include "button.h"

void button_ctrl_init(Controls *c, SDL_Renderer* renderer)
{
    c->arr[MENU] = (Button){img_init(WIDTH - 200, 100, "../sprites/menu_B.png",renderer, true), false};
    c->arr[PLAY] = (Button){img_init(WIDTH - 200, 300, "../sprites/play_B.png",renderer, true), false};
    c->arr[B_HISTORY] = (Button){img_init(WIDTH - 200, 500, "../sprites/history_B.png",renderer, true), false};
    c->arr[PREV_BW] = (Button){img_init(WIDTH - 400,HEIGHT - 300, "../sprites/arrow_L.png",renderer, false), false};
    c->arr[PREV_FW] = (Button){img_init(WIDTH - 200,HEIGHT - 300, "../sprites/arrow_R.png",renderer, false), false};
    c->arr[PERSON] = (Button){img_init(800, 200, "../sprites/person.png",renderer, false), false};
    c->arr[ROBOT] = (Button){img_init(1000, 200, "../sprites/robot.png",renderer, false), false};
    c->arr[B_BLACK] = (Button){img_init(800, 200, "../sprites/disk_B.png",renderer, false), false};
    c->arr[B_WHITE] = (Button){img_init(1000, 200, "../sprites/disk_W.png",renderer, false), false};
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

bool button_tasks(Controls *c, GameList **listp, Game *g, GameList **mover, Master *m)
{
    if (m->ask)
    {
        font_render(m->renderer, (pos){180, 100}, "biztos?");
        c->arr[YES].img.visible = true;
        c->arr[NO].img.visible = true;
    }
    else
    {
        c->arr[YES].img.visible = false;
        c->arr[NO].img.visible = false;
    }
    if (m->state == GAME)
    {
        if (g->state == MATCH || g->state == PREV)
        {
            c->arr[PREV_BW].img.visible = true;
            c->arr[SAVE].img.visible = true;
            c->arr[PREV_FW].img.visible = true;
            c->arr[B_NEW].img.visible = true;
            if (c->arr[PREV_BW].pressed)
            {
                if (g->history_board->former != NULL)
                {
                    g->history_board = g->history_board->former;
                    g->state = PREV;
                }
                c->arr[PREV_BW].pressed = false;
            }
            else if (c->arr[PREV_FW].pressed)
            {
                if (g->history_board->next != NULL)
                {
                    g->history_board = g->history_board->next;
                }
                if (g->history_board->next == NULL) // looks bad, pretty sure can be done better
                {
                    g->state = MATCH;
                }
                c->arr[PREV_FW].pressed = false;
            }
            else if (c->arr[B_NEW].pressed)
            {
                m->ask = true;
                if (c->arr[YES].pressed)
                {
                    game_list_bwdestroy(g->list);
                    game_init(g);
                    c->arr[YES].pressed = false;
                }
                else if (c->arr[NO].pressed)
                {
                    c->arr[NO].pressed = false;
                }
                else
                    return true;

                c->arr[B_NEW].pressed = false;
                m->ask = false;
            }
            else if (c->arr[SAVE].pressed)
            {
                gamelist_save(listp, g, m);
                *mover = *listp;
                c->arr[SAVE].pressed = false;
                return true;
            }
            if (g->state == PREV)
            {
                c->arr[LOAD].img.visible = true;
                if (c->arr[LOAD].pressed)
                {
                    m->ask = true;
                    if (c->arr[YES].pressed)
                    {
                        g->list = g->history_board;
                        game_list_fwdestroy(g->history_board->next);
                        g->history_board->next = NULL;
                        g->state = MATCH;
                        c->arr[YES].pressed = false;
                    }
                    else if (c->arr[NO].pressed)
                    {
                        c->arr[NO].pressed = false;
                    }
                    else
                        return true;
                    m->ask = false;
                    c->arr[LOAD].pressed = false;
                }
            }
            else
            {
                c->arr[LOAD].img.visible = false;
            }
        }
        else
        {
            c->arr[PREV_FW].img.visible = false;
            c->arr[PREV_BW].img.visible = false;
            c->arr[B_NEW].img.visible = false;
            c->arr[SAVE].img.visible = false;
        }
        if (g->state == OPPONENT)
        {
            c->arr[PERSON].img.visible = true;
            c->arr[ROBOT].img.visible = true;
            if (c->arr[PERSON].pressed)
            {
                g->opponent = HUMAN;
                g->state = MATCH;
                game_add_position(g);
                board_default(&g->list->board);
                board_set_valid(&g->list->board);
                c->arr[PERSON].pressed = false;
            }
            else if (c->arr[ROBOT].pressed)
            {
                g->opponent = AI;
                g->state = COLOR;
                c->arr[ROBOT].pressed = false;
            }
            else
                return true;
            c->arr[PERSON].img.visible = false;
            c->arr[ROBOT].img.visible = false;

            return true;
        }
        if (g->state == COLOR)
        {
            c->arr[B_BLACK].img.visible = true;
            c->arr[B_WHITE].img.visible = true;
            if (c->arr[B_BLACK].pressed)
            {
                g->player_color = BLACK;
                c->arr[B_BLACK].pressed = false;
            }

            else if (c->arr[B_WHITE].pressed)
            {
                g->player_color = WHITE;
                c->arr[B_WHITE].pressed = false;
            }

            else
                return true;
            g->state = MATCH;
            board_default(&g->list->board);
            board_set_valid(&g->list->board);
            c->arr[B_BLACK].img.visible = false;
            c->arr[B_WHITE].img.visible = false;

            return true;
        }
    }
    if (m->state == HISTORY && *mover != NULL)
    {
        c->arr[HISTORY_FW].img.visible = true;
        c->arr[DELETE].img.visible = true;
        c->arr[CONT].img.visible = true;
        c->arr[HISTORY_BW].img.visible = true;
        if (c->arr[HISTORY_FW].pressed && (*mover)->next != NULL)
        {
            (*mover) = (*mover)->next;
        }
        if (c->arr[HISTORY_BW].pressed && (*mover)->former != NULL)
        {
            (*mover) = (*mover)->former;
        }
        if (c->arr[CONT].pressed)
        {
            m->ask = true;
            if (c->arr[YES].pressed)
            {
                game_list_bwdestroy(g->list);
                game_cpy(g, (*mover)->game);
                c->arr[YES].pressed = false;
            }
            else if (c->arr[NO].pressed)
            {
                c->arr[NO].pressed = false;
            }
            else
                return true;
            m->ask = false;

            c->arr[CONT].pressed = false;
        }
        if (c->arr[DELETE].pressed)
        {
            m->ask = true;
            if (c->arr[YES].pressed)
            {
                gamelist_remove(mover);
                if (*mover == NULL || (*mover)->next == NULL) *listp = *mover;
                gamelist_fprint(listp);
                c->arr[YES].pressed = false;
            }
            else if (c->arr[NO].pressed)
            {
                c->arr[NO].pressed = false;
            }
            else
                return true;
            m->ask = false;
            c->arr[DELETE].pressed = false;
        }
        c->arr[HISTORY_BW].pressed = false;
        c->arr[HISTORY_FW].pressed = false;
    }
    else
    {
        c->arr[DELETE].img.visible = false;
        c->arr[CONT].img.visible = false;
        c->arr[HISTORY_FW].img.visible = false;
        c->arr[HISTORY_BW].img.visible = false;
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