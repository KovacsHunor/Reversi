#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>

typedef struct pos{
    int x;
    int y;
}pos;

pos pos_add(pos p1, pos p2);

bool pos_hover(pos frame, pos size, pos point);

#endif