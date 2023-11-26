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

void gamelist_remove(GameList **list)
{
    if(*list == NULL) return;
    GameList *temp = *list;

    if (temp->next != NULL){
        temp->next->former = temp->former;
        *list = temp->next;
    }
    else{
        *list = temp->former;
    }
    if (temp->former != NULL)
        temp->former->next = temp->next;
    game_destroy(temp->game);
    free(temp);
    gamelist_fprint(list);
}

void gamelist_tolast(GameList **list)
{
    while (*list != NULL && (*list)->next != NULL)
        *list = (*list)->next;
}

void gamelist_load(Game *g, GameList **list){
    game_list_bwdestroy(g->list);
    game_cpy(g, (*list)->game);
}

void gamelist_save(GameList **list, Game *g)
{
    Game *game = (Game *)malloc(sizeof(Game));
    game_cpy(game, g);
    game->date = time(NULL);
    gamelist_add(list, game);
    gamelist_tolast(list);
    gamelist_fprint(list);
}

void gamelist_fread(GameList **list){
    FILE *fp = fopen("../save/save.txt", "r");
    char input;
    int enumbuffer;
    while (fscanf(fp, "%c", &input) != EOF)
    {
        if(*list == NULL){
            *list = (GameList*)malloc(sizeof(GameList));
            (*list)->next = NULL;
        }
        else{
            (*list)->former = (GameList*)malloc(sizeof(GameList));
            (*list)->former->next = *list;
            *list = (*list)->former;
        }
        (*list)->former = NULL;
        (*list)->game = (Game*)malloc(sizeof(Game));
        *(*list)->game = (Game){.list = NULL};
        fscanf(fp, "%c%ld", &input, &(*list)->game->date);
        fscanf(fp, "%c%d", &input, &enumbuffer);
        (*list)->game->opponent = enumbuffer;
        fscanf(fp, "%c%d", &input, &enumbuffer);
        (*list)->game->player_color = enumbuffer;
        fscanf(fp, "%c", &input);
        while (input != '!')
        {
            //cikluson belül kell legyen, mert nem tudni, mikor jön vége jelzés (!)
            if((*list)->game->list == NULL){
                (*list)->game->list = (BoardList *)malloc(sizeof(BoardList));
                (*list)->game->list->next = NULL;
            }
            else{
                (*list)->game->list->former = (BoardList *)malloc(sizeof(BoardList));
                (*list)->game->list->former->next = (*list)->game->list;
                (*list)->game->list = (*list)->game->list->former;
            }
            (*list)->game->list->former = NULL;
            (*list)->game->list->board = board_init();  // boardlength, tilecount -> higher struct (master?)
            fscanf(fp, "%c%d", &input, &enumbuffer);
            (*list)->game->list->board.state = enumbuffer;
            fscanf(fp, "%c%d", &input, &enumbuffer);
            (*list)->game->list->board.side = enumbuffer;
            fscanf(fp, "%c%d", &input, &(*list)->game->list->board.valid_count);

            fscanf(fp, "%c%d", &input, &(*list)->game->list->board.points[0]);
            fscanf(fp, "%c%d", &input, &(*list)->game->list->board.points[1]);

            for (int i = 0; i < TILECOUNT; i++)
            {
                for (int j = 0; j < TILECOUNT; j++)
                {
                    fscanf(fp, "%c%d", &input, &enumbuffer);
                    (*list)->game->list->board.disks[i][j] = enumbuffer;
                }
            }
            fscanf(fp, "%c", &input);
        }
        game_tolast(&(*list)->game->list);
    }
    gamelist_tolast(list);
    fclose(fp);
}

void gamelist_fprint(GameList **list)
{
    FILE *fp = fopen("../save/save.txt", "w+");
    GameList *temp = *list;
    while (temp != NULL)
    {
        unsigned int enumbuffer;
        fprintf(fp, "__%ld_%d_%d", temp->game->date, temp->game->opponent, temp->game->player_color);

        BoardList *btemp = temp->game->list;
        while (btemp != NULL)
        {
            fprintf(fp, "__%d", btemp->board.state);
            fprintf(fp, "_%d", btemp->board.side);
            fprintf(fp, "_%d", btemp->board.valid_count);
            fprintf(fp, "_%d", btemp->board.points[0]);
            fprintf(fp, "_%d", btemp->board.points[1]);
            for (int i = 0; i < TILECOUNT; i++)
            {
                for (int j = 0; j < TILECOUNT; j++)
                {
                    fprintf(fp, "_%d", btemp->board.disks[i][j]);
                }
            }
            btemp = btemp->former;
        }
        fprintf(fp, "!");
        temp = temp->former;
    }

    fclose(fp);
}
