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

void board_destruct(Board* b){
    for (int i = 0; i < b->tile_count; i++)
    {
        free(b->disks[i]);
    }
    free(b->disks);
}

void board_clear_board(Board* b,  Master* m){
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b-> tile_count; j++)
        {
            b->disks[i][j] = board_create_disk(NONE, m);
        }
    }
}

Disk board_create_disk(disk_color color, Master* m)
{
    if(color == WHITE) return (Disk){color, img_init(0, 0, "../sprites/disk_W.png", m, true)};
    if(color == BLACK) return (Disk){color, img_init(0, 0, "../sprites/disk_B.png", m, true)};
    return (Disk){color};
};

void board_draw(Master* m, Board* b)
{
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            SDL_Rect r = {i * (b->tile_size + 1) + b->x, j * (b->tile_size + 1) + b->y, b->tile_size, b->tile_size};
            SDL_SetRenderDrawColor(m->renderer, 0, (i + j) % 2 ? 120 : 130, 0, 255);
            SDL_RenderFillRect(m->renderer, &r);
        }
    }
    SDL_SetRenderDrawColor(m->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(m->renderer);
}