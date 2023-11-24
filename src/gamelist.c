#include "gamelist.h"

void gamelist_add(GameList **list, Game *g)
{   
    GameList *new = (GameList *)malloc(sizeof(GameList));
    new->game = g;
    new->former = *list;
    new->next = NULL;
    if (*list != NULL)
        (*list)->next = new;
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

void gamelist_remove(GameList **list_item){
    GameList* temp = *list_item;

    if((*list_item)->former != NULL) (*list_item)->former->next = (*list_item)->next;
    if((*list_item)->next != NULL) (*list_item)->next->former = (*list_item)->former;

    if((*list_item)->former != NULL) *list_item = (*list_item)->former;
    else *list_item = (*list_item)->next;
    game_destroy(temp->game);
    free(temp);
}

void gamelist_tofirst(GameList** list){
    while((*list)->next != NULL) *list = (*list)->next;
}

void gamelist_save(GameList** list, Game* g, Master* m){
    Game* game = (Game*)malloc(sizeof(Game));
    game_cpy(game, g);
    game->date = time(NULL);
    gamelist_add(list, game);
    gamelist_tofirst(list);
}
