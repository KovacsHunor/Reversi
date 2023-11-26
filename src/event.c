#include "event.h"

void event_pressed_confirm(Button *controls, bool *ask, Game *g, Button_id id, GameList **list)
{
    if (!*ask)
    {
        event_askflip(controls, ask);
    }
    *ask = true;
    if (controls[YES].pressed)
    {
        switch (id)
        {
        case B_NEW:
            game_tolast(&g->list);
            game_list_bwdestroy(g->list);
            game_init(g);
            break;
        case CUT:
            game_cut(g);
            break;
        case LOAD:
            gamelist_load(g, list);
            break;
        case DELETE:
            gamelist_remove(list);
            break;
        default:
            break;
        }
        controls[YES].pressed = false;
    }
    else if (controls[NO].pressed)
    {
        controls[NO].pressed = false;
    }
    else
        return;
    controls[id].pressed = false;
    event_askflip(controls, ask);
}

void event_basic(Button *controls, GameList **list, Game *g, Master *m)
{
    controls[PREV_BW].img.visible = true;
    controls[SAVE].img.visible = true;
    controls[PREV_FW].img.visible = true;
    controls[B_NEW].img.visible = true;
    if (controls[PREV_BW].pressed)
    {
        if (g->list->former != NULL)
        {
            g->list = g->list->former;
            g->state = PREV;
        }
        controls[PREV_BW].pressed = false;
    }
    else if (controls[PREV_FW].pressed)
    {
        if (g->list->next != NULL)
            g->list = g->list->next;
        if (g->list->next == NULL)
            g->state = MATCH;
        controls[PREV_FW].pressed = false;
    }
    else if (controls[B_NEW].pressed)
        event_pressed_confirm(controls, &m->ask, g, B_NEW, list);
    else if (controls[SAVE].pressed)
    {
        gamelist_save(list, g);
        controls[SAVE].pressed = false;
    }
    if (g->state == PREV)
    {
        controls[CUT].img.visible = true;
        if (controls[CUT].pressed)
            event_pressed_confirm(controls, &m->ask, g, CUT, list);
    }
    else
        controls[CUT].img.visible = false;
}

void event_opponent(Button *controls, GameList **list, Game *g)
{
    controls[PERSON].img.visible = true;
    controls[ROBOT].img.visible = true;
    if (controls[PERSON].pressed)
    {
        g->opponent = HUMAN;
        g->state = MATCH;
        board_default(&g->list->board);
        board_set_valid(&g->list->board);
        controls[PERSON].pressed = false;
    }
    else if (controls[ROBOT].pressed)
    {
        g->opponent = AI;
        g->state = COLOR;
        controls[ROBOT].pressed = false;
    }
    else
        return;
    controls[PERSON].img.visible = false;
    controls[ROBOT].img.visible = false;
}

void event_color(Button *controls, GameList **list, Game *g)
{
    controls[B_BLACK].img.visible = true;
    controls[B_WHITE].img.visible = true;
    if (controls[B_BLACK].pressed)
    {
        g->player_color = BLACK;
        controls[B_BLACK].pressed = false;
    }
    else if (controls[B_WHITE].pressed)
    {
        g->player_color = WHITE;
        controls[B_WHITE].pressed = false;
    }

    else
        return;
    g->state = MATCH;
    board_default(&g->list->board);
    board_set_valid(&g->list->board);
    controls[B_BLACK].img.visible = false;
    controls[B_WHITE].img.visible = false;
}

void event_game(Button *controls, GameList **list, Game *g, Master *m)
{
    if (g->state == MATCH || g->state == PREV)
        event_basic(controls, list, g, m);
    else
    {
        controls[PREV_FW].img.visible = false;
        controls[PREV_BW].img.visible = false;
        controls[B_NEW].img.visible = false;
        controls[SAVE].img.visible = false;
    }
    if (g->state == OPPONENT)
        event_opponent(controls, list, g);
    if (g->state == COLOR)
        event_color(controls, list, g);
}

void event_history(Button *controls, GameList **list, Game *g, bool *ask)
{
    controls[HISTORY_FW].img.visible = true;
    controls[DELETE].img.visible = true;
    controls[LOAD].img.visible = true;
    controls[HISTORY_BW].img.visible = true;
    if (controls[HISTORY_FW].pressed)
    {
        controls[HISTORY_FW].pressed = false;
        if ((*list)->next != NULL)
            (*list) = (*list)->next;
    }
    else if (controls[HISTORY_BW].pressed)
    {
        controls[HISTORY_BW].pressed = false;
        if ((*list)->former != NULL)
            (*list) = (*list)->former;
    }
    else if (controls[LOAD].pressed)
        event_pressed_confirm(controls, ask, g, LOAD, list);
    else if (controls[DELETE].pressed)
        event_pressed_confirm(controls, ask, g, DELETE, list);
}

void event_askflip(Button *controls, bool *ask)
{
    *ask = !*ask;
    controls[YES].img.visible = !controls[YES].img.visible;
    controls[NO].img.visible = !controls[NO].img.visible;
}

void event_history_off(Button *controls)
{
    controls[DELETE].img.visible = false;
    controls[LOAD].img.visible = false;
    controls[HISTORY_FW].img.visible = false;
    controls[HISTORY_BW].img.visible = false;
}

void event_main(Button *controls, GameList **list, Game *g, Master *m)
{
    if (m->state == GAME)
    {
        event_game(controls, list, g, m);
    }
    if (m->state == HISTORY && *list != NULL)
    {
        event_history(controls, list, g, &m->ask);
    }
    else
    {
        event_history_off(controls);
    }
}