#include "gamelist.h"

void gamelist_add(GameList **list, Game *g)
{   
    GameList * temp = *list;
    if(temp != NULL) while(temp->next != NULL) temp = temp->next;
    GameList *new = (GameList *)malloc(sizeof(GameList));
    new->game = g;
    new->former = temp;
    new->next = NULL;
    if (new->former != NULL)
        new->former->next = new;
        *list = new;
}

void gamelist_destroy(GameList *list)
{
    if (list == NULL)
        return;
    gamelist_destroy(list->former);
    game_destroy(list->game);
    free(list);
}

void gamelist_tofirst(GameList** list){
    while((*list)->next != NULL) *list = (*list)->next;
}

void gamelist_new(GameList** list, Master* m){
    Game* game = (Game*)malloc(sizeof(Game));
    game_init(game, BLACK, AI, m);
    gamelist_add(list, game);
    gamelist_tofirst(list);
}
