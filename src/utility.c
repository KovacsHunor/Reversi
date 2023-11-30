#include "utility.h"

pos pos_add(pos p1, pos p2){
    return (pos){p1.x+p2.x, p1.y + p2.y};
}

bool pos_hover(pos frame, pos size, pos point){
    return point.x>=frame.x && point.y>=frame.y && point.x<=frame.x+size.x && point.y<=frame.y+size.y;
}

int max(int a, int b){
    return a>b?a:b;
}

int min(int a, int b){
    return a<b?a:b;
}

void merror(void* pointer){
    if (pointer == NULL)
    {
        perror("Nem sikerült memóriát foglalni!");
        exit(1);
    }
}
