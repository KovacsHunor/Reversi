#include "utility.h"

pos pos_add(pos p1, pos p2){
    return (pos){p1.x+p2.x, p1.y + p2.y};
}

bool pos_hover(pos frame, pos size, pos point){
    return point.x>=frame.x && point.y>=frame.y && point.x<=frame.x+size.x && point.y<=frame.y+size.y;
}