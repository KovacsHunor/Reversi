#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "image.h"
#include "utility.h"
#include "font.h"
#include "debugmalloc.h"
#define TILECOUNT 8

typedef enum b_event{
    BASIC, PASS, END
}b_event;

typedef enum disk_color{
    WHITE, BLACK, VALID, NONE
}disk_color;

typedef struct Disk{
    disk_color color;
    pos p;
}Disk;

typedef struct Board{
    int side[2];
    int tile_size;
    int tile_count;
    int length;
    int x, y;
    int valid_count;
    disk_color turn;
    pos msg;
    Disk disks[8][8];
}Board;

//-
disk_color board_more(Board* b);

//-
Board board_init(int board_length, Master* m);

//-
void board_put_disk(Board* b, pos p, Master* m, disk_color side);

//-
void board_after_move(Board* b, disk_color *side, Master* m, b_event* e);

//-
bool board_raycast(Board* b, pos p, disk_color c, bool flip, Master* m);

//-
bool board_rec_valid(Board* b, pos p, pos v, disk_color c, bool first, bool flip, Master* m);

//-
void board_render_disk(Disk *d, pos p, Master *m);

//-
disk_color board_flip_color(disk_color c);

//-
void board_set_valid(Board* b, disk_color c, Master* m);

//-
void board_print_event(Board *b, Master *m, disk_color c, b_event e);

//-
void board_default(Board* b, Master* m);

//-
void board_set_color(Disk* d, disk_color c, Master* m);

//-
void board_clear(Board* b, Master* m, bool only_valid);

//-
int minimax(Board* b, int depth, int alpha, int beta, bool maximizing, Master* m);

//-
Disk board_create_disk(int x, int y, disk_color color, Master* m);

//-
void board_render(Master* m, Board* b, disk_color c, b_event e);

#endif