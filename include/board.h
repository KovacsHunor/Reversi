#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "image.h"
#include "utility.h"
#include "font.h"
#include "debugmalloc.h"

typedef enum b_state{
    BASIC, PASS, END
}b_state;

typedef enum Disk{
    WHITE, BLACK, VALID, NONE
}Disk;

typedef struct Board{
    int points[2];

    pos position;
    int valid_count;

    Disk side;
    pos msg;

    b_state state;
    Disk disks[8][8];
}Board;

//-
Disk board_more(Board* b);

int minimax(Board *b, int depth, int alpha, int beta);

//-
Board board_init();

void board_put_disk(Board *b, pos p);

//-
bool board_raycast(Board* b, pos p, bool flip);

//-
bool board_rec_valid(Board* b, pos p, pos v, bool first, bool flip);

//-
void board_render_disk(Disk d, pos p, SDL_Renderer* renderer);

void board_clear_valid(Board *b);

//-
Disk board_flip_color(Disk c);

void board_after_move(Board *b);

//-
void board_set_valid(Board* b);

//-
void board_print_event(Board *b, SDL_Renderer* renderer);

//-
void board_default(Board* b);

void board_make(Board *b);

//-
void board_render(SDL_Renderer* renderer, Board* b);

#endif