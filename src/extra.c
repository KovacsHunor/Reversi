#include "extra.h"

pos pos_add(pos p1, pos p2){
    return (pos){p1.x+p2.x, p1.y + p2.y};
}