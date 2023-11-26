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


/// @brief pozíció tárolására alkalmas típus
typedef struct pos{
    int x;
    int y;
}pos;


/// @brief visszatér két pozíció összegével
/// @param p1 1. pozíció
/// @param p2 2. pozíció
/// @return összeg
pos pos_add(pos p1, pos p2);


/// @brief megállapítja, hogy két pozíció által meghatározott területen belül van-e a harmadik
/// @param frame a terület bal felső sarka
/// @param size a terület méretei
/// @param point a pont helye
/// @return IGAZ ha benne van
bool pos_hover(pos frame, pos size, pos point);


/// @brief két egész közül a nagyobbal tér vissza
/// @param a egyik egész
/// @param b másik egész
/// @return nagyobbik egész
int max(int a, int b);


/// @brief két egész közül a kisebbel tér vissza
/// @param a egyik egész
/// @param b másik egész
/// @return kisebb egész
int min(int a, int b);

#endif