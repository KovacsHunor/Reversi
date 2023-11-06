#include "board.h"
#define TILECOUNT 8

Board board_init(int board_length, Master* m){
    return (Board){.tile_count = TILECOUNT, .tile_size = board_length/TILECOUNT, .x = m->width/2 - board_length/2, .y = m->height/2 - board_length/2};
}

void board_disks_init(Board* b){
    b->disks = (Disk**)malloc(b->tile_count*sizeof(Disk*));
    for (int i = 0; i < b->tile_count; i++)
    {
        b->disks[i] = (Disk*)malloc(b->tile_count*sizeof(Disk));
    }
}

void board_default(Board* b, Master* m){
    for (int i = 0; i < b->tile_count; i++){
        for (int j = 0; j < b->tile_count; j++)
        {
            b->disks[i][j] = board_create_disk(b->x + i*(b->tile_size + 1), b->y + j*(b->tile_size + 1), NONE, m);
        }
    }
    board_set_color(&b->disks[3][4], WHITE, m);
    board_set_color(&b->disks[4][3], WHITE, m);
    board_set_color(&b->disks[3][3], BLACK, m);
    board_set_color(&b->disks[4][4], BLACK, m);
}

void board_event(Board* b, int x, int y, disk_color* side, Master* m){
    for (int i = 0; i < b->tile_count; i++){
        for (int j = 0; j < b->tile_count; j++)
        {
            if(b->disks[i][j].color == VALID && img_hover(&b->disks[i][j].img, x, y)){
                board_clear(b, m, true);
                board_set_color(&b->disks[i][j], *side, m);
                board_raycast(b, (pos){j, i}, *side, true, m);
                *side = board_flip_color(*side);
                board_set_valid(b, *side, m);
                board_render(m, b);
                SDL_RenderPresent(m->renderer);

                return;
            }
        }
    }
}

void board_destruct(Board* b){
    for (int i = 0; i < b->tile_count; i++)
    {
        free(b->disks[i]);
    }
    free(b->disks);
}

disk_color board_flip_color(disk_color c){
    if(c == WHITE) return BLACK;
    if(c == BLACK) return WHITE;
    return c;
}

bool board_rec_valid(Board* b, pos p, pos v, disk_color c, bool first, bool flip, Master* m){
    if(p.x>=0 && p.x < b->tile_count && p.y>=0 && p.y<b->tile_count){
        disk_color current = b->disks[p.y][p.x].color;
        if(!first && current == c) return true;
        if(current == board_flip_color(c)){
            bool found = board_rec_valid(b, pos_add(p, v), v, c, false, flip, m);
            if(found && flip) board_set_color(&b->disks[p.y][p.x], c, m);
            return found;
        }
    }
    return false;
}

bool board_raycast(Board* b, pos p, disk_color c, bool flip, Master* m){
    bool valid = false;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if(i != 0 || j != 0){
                pos v = {i, j};
                valid |= board_rec_valid(b, pos_add(p, v), v, c, true, flip, m);
            }
        }
    }
    return valid;
}

void board_set_valid(Board* b, disk_color c, Master* m){
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            if(b->disks[i][j].color == NONE && board_raycast(b, (pos){j, i}, c, false, m)){
                board_set_color(&b->disks[i][j], VALID, m);
            }
            printf("%d ", b->disks[i][j].color);
        }
        printf("\n");
    }
}

void board_set_color(Disk* d, disk_color c, Master* m){
    d->color = c;
    if(c == WHITE){ d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/disk_W.png"); return;}
    if(c == BLACK){ d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/disk_B.png"); return;}
    if(c == VALID){ d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/disk_V.png"); return;}
    d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/none.png");
}

void board_clear(Board* b, Master* m, bool only_valid){
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            if(only_valid && b->disks[i][j].color != VALID) continue;
            board_set_color(&b->disks[i][j], NONE, m);
        }
    }
}

Disk board_create_disk(int x, int y, disk_color c, Master* m)
{
    Disk d = {c, img_init(x, y, "../sprites/disk_B.png", m, true)};
    board_set_color(&d, c, m);
    return d;
};

void board_render(Master* m, Board* b)
{
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            SDL_Rect r = {i * (b->tile_size + 1) + b->x, j * (b->tile_size + 1) + b->y, b->tile_size, b->tile_size};
            SDL_SetRenderDrawColor(m->renderer, 0, (i + j) % 2 ? 120 : 130, 0, 255);
            SDL_RenderFillRect(m->renderer, &r);

            if(b->disks[i][j].color != NONE){
                img_render(&b->disks[i][j].img, m);
            }
        }
    }
    SDL_SetRenderDrawColor(m->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(m->renderer);
}