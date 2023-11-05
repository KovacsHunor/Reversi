#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>

#include "image.h"

typedef enum disk_color{
    WHITE, BLACK, NONE
}disk_color;

typedef struct Disk{
    disk_color color;
    Image img;
}Disk;

typedef struct Board{
    int tile_size;
    int tile_count;
    int x, y;
    Disk** disks;
}Board;

Board board_init(int board_length, Master* m);

//allocates memory for the board, has to be freed after the game finished
void board_disks_init(Board* b);

Disk board_create_disk(disk_color color, Master* m);

void board_destruct(Board* b);

void board_disks_default(Board* b);

void board_render(Master* m, Board* b);

#endif