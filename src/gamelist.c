#include "gamelist.h"

void gamelist_add(GameList **list, Game *g)
{
    GameList *new = (GameList *)malloc(sizeof(GameList));
    new->game = *g;
    new->former = *list;
    if (*list != NULL)
    {
        while (new->former->former != NULL && g->date > new->former->game.date)
        {
            new->former = new->former->former;
        }
        if(new->former->next == NULL){
            new->former->next = new;
            new->next = NULL;
            *list = new;
        }
        else if(new->former->former == NULL){
            new->former->former = new;
            new->next = new->former;
            new->former = NULL;
        }
        else{
            new->next = new->former->next;
            new->next->former = new;
            new->former->next = new;
        }
    }
    else
    {
        new->next = NULL;
        *list = new;
    }
}

void gamelist_destroy(GameList *list)
{
    if (list == NULL)
        return;
    game_list_destroy(list->next);
    free(list);
}