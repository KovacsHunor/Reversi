#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "image.h"
#include "utility.h"
#include "font.h"
//#include "debugmalloc.h"
#define TILECOUNT 8

typedef enum b_state{
    BASIC, PASS, END
}b_state;

typedef enum disk_color{
    WHITE, BLACK, VALID, NONE
}disk_color;

typedef struct Disk{
    disk_color color;
    pos p;
}Disk;

typedef struct Board{
    int points[2];
    int tile_size;
    int tile_count;
    int length;

    pos position;
    int valid_count;

    disk_color side;
    pos msg;

    b_state state;
    disk_color disks[8][8];
}Board;

//-
disk_color board_more(Board* b);

//-
Board board_init(int board_length, Master* m);

//-
void board_make(Board* b, Master *m);

//-
void board_put_disk(Board* b, pos p, Master* m);

//-
void board_after_move(Board* b, Master* m);

//-
bool board_raycast(Board* b, pos p, bool flip, Master* m);

//-
bool board_rec_valid(Board* b, pos p, pos v, bool first, bool flip, Master* m);

//-
void board_render_disk(disk_color d, pos p, Master *m);

//-
disk_color board_flip_color(disk_color c);

//-
void board_set_valid(Board* b, Master* m);

//-
void board_print_event(Board *b, Master *m);

//-
void board_default(Board* b, Master* m);

//-
void board_clear(Board* b, Master* m, bool only_valid);

//-
int minimax(Board* b, int depth, int alpha, int beta, Master* m);


//-
void board_render(Master* m, Board* b);

#endif