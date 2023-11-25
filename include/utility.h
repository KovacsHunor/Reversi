#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>
#include <time.h>
#include "debugmalloc.h"

#define WIDTH 1920
#define HEIGHT 1080
#define TILECOUNT 8
#define TILESIZE 100
#define BOARDLENGTH TILECOUNT*(TILESIZE+1)
#define BOARDX 500
#define BOARDY 100


typedef struct pos{
    int x;
    int y;
}pos;

//-
pos pos_add(pos p1, pos p2);

//-
bool pos_hover(pos frame, pos size, pos point);

//-
int max(int a, int b);

//-
int min(int a, int b);

#endif