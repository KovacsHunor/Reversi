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
            b->disks[i][j] = board_create_disk(b->x + i*(b->tile_size + 1), b->y + j*(b->tile_size + 1), 3<=i&&i<=4&&3<=j&&j<=4 ? true : false, BLACK, m);
        }
    }
    board_color_swap(&b->disks[3][4]);
    board_color_swap(&b->disks[4][3]);
}

void board_destruct(Board* b){
    for (int i = 0; i < b->tile_count; i++)
    {
        free(b->disks[i]);
    }
    free(b->disks);
}

void board_color_swap(Disk* d){
    if(d->color == WHITE) d->color = BLACK;
    else if(d->color == BLACK) d->color = WHITE;
    img_texture_swap(&d->sw_sprite, &d->img.sprite);
}

void board_clear_board(Board* b,  Master* m){
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            b->disks[i][j].img.visible = false;
        }
    }
}

Disk board_create_disk(int x, int y, bool visible, disk_color color, Master* m)
{
    Disk d = {BLACK, IMG_LoadTexture(m->renderer, "../sprites/disk_B.png"), img_init(x, y, "../sprites/disk_W.png", m, visible)};
    if(color == WHITE) board_color_swap(&d);
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

            if(b->disks[i][j].img.visible){
                img_render(&b->disks[i][j].img, m);
            }
        }
    }
    SDL_SetRenderDrawColor(m->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(m->renderer);
}