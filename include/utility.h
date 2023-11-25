#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>
#include <time.h>
//#include "debugmalloc.h"

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