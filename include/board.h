#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "image.h"
#include "utility.h"
#include "ffont.h"
#include "debugmalloc.h"

/// @brief az állás állapota
typedef enum Board_state{
    BASIC, PASS, END
}Board_state;


/// @brief egy korong típusa
typedef enum Disk{
    WHITE, BLACK, VALID, NONE
}Disk;


/// @brief egy állás
typedef struct Board{
    int points[2];

    pos position;
    int valid_count;

    Disk side;
    pos msg;

    Board_state state;
    Disk disks[8][8];
}Board;


/// @brief visszaadja, hogy kinek van több pontja adott állásban
/// @param b az állás
/// @return a magasabb pontszámú fél
static Disk board_more(Board* b);

/// @brief állás adott pozíciójából egyenes irányokban végigmegy az állás elemein
/// @param b állás
/// @param p pozíció
/// @param flip módosítson-e a megtalált fordítható elemeken
/// @return talált-e valid pozíciót
static bool board_raycast(Board* b, pos p, bool flip);

/// @brief állás adott pozíciójából adott irányban rekurzívan bejár
/// @param b állás
/// @param p pozíció
/// @param vector az irányt adó vektor
/// @param first először hívódott-e meg a függvény
/// @param flip módosítson-e a megtalált fordítható elemeken
/// @return talált-e valid pozíciót
static bool board_rec_valid(Board* b, pos p, pos vector, bool first, bool flip);

/// @brief rendereli az adott pozícióban lévő koronghoz tartozó képet
/// @param d korong
/// @param p pozíció
/// @param renderer a renderer
static void board_render_disk(Disk d, pos p, SDL_Renderer* renderer);

/// @brief adott állásban lévő valid típusú korongokat none típusúakra cseréli
/// @param b állás
static void board_clear_valid(Board *b);

/// @brief kirendereli szöveges formában az állás állapotát
/// @param b állás
/// @param renderer a renderer
static void board_print_state(Board *b, SDL_Renderer* renderer);

/// @brief adott állásra rekurzívan megtalálja a legjobb lépést, adott mélységig nézve, alpha-béta vágással gyorsítva
/// @param b az állás
/// @param depth mélység
/// @param alpha alpha érték
/// @param beta béta érték
/// @return az álláshoz rendelt érték
int minimax(Board *b, int depth, int alpha, int beta);

/// @brief visszaad egy üres állást, alapvető adatokkal feltöltve
/// @return üres állás
Board board_init();

/// @brief állás adott pozíciójára lerak egy korongot, és megfelelően módosítja az állást
/// @param b állás
/// @param p pozíció
void board_put_disk(Board *b, pos p);

/// @brief visszaadja a korong fordítottját
/// @param disk a korong
/// @return a korong fordítottja
Disk board_flip_color(Disk disk);

/// @brief elvégzi egy lépés után szükséges utó-feladatokat: frissíti az állás állapotát
/// @param b az állás
void board_after_move(Board *b);

/// @brief meghatározza a valid lépéseket adott állásban
/// @param b az állás
void board_set_valid(Board* b);

/// @brief adott állást alapfelállásra állít
/// @param b az állás
void board_default(Board* b);

/// @brief none típusú korongokkal tölti fel az állást
/// @param b az állás
void board_clear(Board *b);

/// @brief rendereli az állást
/// @param renderer a renderer
/// @param b az állás
/// @param player a játékos színe
void board_render(SDL_Renderer* renderer, Board* b, Disk player);

#endif