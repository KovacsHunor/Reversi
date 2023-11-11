#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "image.h"
#include "utility.h"
#include "font.h"

typedef enum disk_color{
    WHITE, BLACK, VALID, NONE
}disk_color;

typedef struct Disk{
    disk_color color;
    Image img;
}Disk;

typedef struct Board{
    int side[2];
    int tile_size;
    int tile_count;
    int length;
    int x, y;
    int valid_count;
    pos msg;
    Disk** disks;
}Board;

//-
disk_color board_more(Board* b);

//-
Board board_init(int board_length, Master* m);

//-
void board_event(Board* b, int x, int y, disk_color* side, Master* m);

//-
bool board_raycast(Board* b, pos p, disk_color c, bool flip, Master* m);

//-
bool board_rec_valid(Board* b, pos p, pos v, disk_color c, bool first, bool flip, Master* m);

//-
disk_color board_flip_color(disk_color c);

//-
void board_set_valid(Board* b, disk_color c, Master* m);

//allocates memory for the board, has to be freed after the game finished
void board_disks_init(Board* b);

//-
void board_default(Board* b, Master* m);

//-
void board_set_color(Disk* d, disk_color c, Master* m);

//-
void board_clear(Board* b, Master* m, bool only_valid);

//-
Disk board_create_disk(int x, int y, disk_color color, Master* m);

//-
void board_destruct(Board* b);

//-
void board_render(Master* m, Board* b);

#endif