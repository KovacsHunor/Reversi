#include "event.h"

void event_pressed_confirm(Controls *c, bool* ask, Game *g, button_id id, GameList **list)
{
    *ask = true;
    if (c->arr[YES].pressed)
    {
        switch (id)
        {
        case B_NEW:
            event_new(g);
            break;
        case LOAD:
            event_load(g);
            break;
        case CONT:
            event_cont(g, list);
            break;
        case DELETE:
            event_remove(list);
            break;
        default:
            break;
        }
        c->arr[YES].pressed = false;
    }
    else if (c->arr[NO].pressed)
    {
        c->arr[NO].pressed = false;
    }
    else
        return;
    c->arr[id].pressed = false;
    *ask = false;
}

void event_new(Game *g)
{
    game_list_bwdestroy(g->list);
    game_init(g);
}

void event_load(Game *g)
{
    g->list = g->history_board;
    game_list_fwdestroy(g->history_board->next);
    g->history_board->next = NULL;
    g->state = MATCH;
}

void event_cont(Game *g, GameList **list)
{
    game_list_bwdestroy(g->list);
    game_cpy(g, (*list)->game);
}

void event_remove(GameList **list)
{
    gamelist_remove(list);
    gamelist_fprint(list);
}

void event_basic(Controls *c, GameList **list, Game *g, Master *m)
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
            g->history_board = g->history_board->next;
        if (g->history_board->next == NULL)
            g->state = MATCH;
        c->arr[PREV_FW].pressed = false;
    }
    else if (c->arr[B_NEW].pressed)
        event_pressed_confirm(c, &m->ask, g, B_NEW, list);
    else if (c->arr[SAVE].pressed)
    {
        gamelist_save(list, g, m);
        c->arr[SAVE].pressed = false;
    }
    if (g->state == PREV)
    {
        c->arr[LOAD].img.visible = true;
        if (c->arr[LOAD].pressed)
            event_pressed_confirm(c, &m->ask, g, LOAD, list);
    }
    else
        c->arr[LOAD].img.visible = false;
}

void event_opponent(Controls *c, GameList **list, Game *g)
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
        return;
    c->arr[PERSON].img.visible = false;
    c->arr[ROBOT].img.visible = false;
}

void event_color(Controls *c, GameList **list, Game *g)
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
        return;
    g->state = MATCH;
    board_default(&g->list->board);
    board_set_valid(&g->list->board);
    c->arr[B_BLACK].img.visible = false;
    c->arr[B_WHITE].img.visible = false;
}

void event_game(Controls *c, GameList **list, Game *g, Master *m)
{
    if (g->state == MATCH || g->state == PREV)
        event_basic(c, list, g, m);
    else
    {
        c->arr[PREV_FW].img.visible = false;
        c->arr[PREV_BW].img.visible = false;
        c->arr[B_NEW].img.visible = false;
        c->arr[SAVE].img.visible = false;
    }
    if (g->state == OPPONENT)
        event_opponent(c, list, g);
    if (g->state == COLOR)
        event_color(c, list, g);
}

void event_history(Controls *c, GameList **list, Game *g, Master *m)
{
    c->arr[HISTORY_FW].img.visible = true;
    c->arr[DELETE].img.visible = true;
    c->arr[CONT].img.visible = true;
    c->arr[HISTORY_BW].img.visible = true;
    if (c->arr[HISTORY_FW].pressed)
    {
        c->arr[HISTORY_FW].pressed = false;
        if ((*list)->next != NULL)
            (*list) = (*list)->next;
    }
    else if (c->arr[HISTORY_BW].pressed)
    {
        c->arr[HISTORY_BW].pressed = false;
        if ((*list)->former != NULL)
            (*list) = (*list)->former;
    }
    else if (c->arr[CONT].pressed)
        event_pressed_confirm(c, &m->ask, g, CONT, list);
    else if (c->arr[DELETE].pressed)
        event_pressed_confirm(c, &m->ask, g, DELETE, list);
}

void event_main(Controls *c, GameList **list, Game *g, Master *m)
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
        event_game(c, list, g, m);
    }
    if (m->state == HISTORY && *list != NULL)
    {
        event_history(c, list, g, m);
    }
    else
    {
        c->arr[DELETE].img.visible = false;
        c->arr[CONT].img.visible = false;
        c->arr[HISTORY_FW].img.visible = false;
        c->arr[HISTORY_BW].img.visible = false;
    }
}