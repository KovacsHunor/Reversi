#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "image.h"
#include "master.h"
#include "debugmalloc.h"
#include "game.h"
#include "gamelist.h"

/// @brief a vezérlőgombok azonosítói
typedef enum Button_id
{
    MENU,
    PLAY,
    B_HISTORY,
    PREV_FW,
    PREV_BW,
    PERSON,
    ROBOT,
    B_BLACK,
    B_WHITE,
    B_NEW,
    HISTORY_FW,
    HISTORY_BW,
    CUT,
    LOAD,
    DELETE,
    SAVE,
    YES,
    NO,
    SIZE
} Button_id;

/// @brief gomb típus
typedef struct Button
{
    Image img;
    bool pressed;
} Button;

/// @brief felszabadítja a kapott gomb-tömb elemeit
/// @param b gombok tömbje
void button_ctrl_destroy(Button* b);

/// @brief megnézi, hogy megnyomható-e az adott gomb
/// @param b a gombok tömbje
/// @param id a gomb azonosítója
/// @param p a kattintás helye
/// @return megnyomható-e
bool button_pressable(Button *b, Button_id id, pos p);

/// @brief a menü gombokon kívül minden gombot eltűntet
/// @param b a gombok tömbje
void button_ctrl_minimal(Button *b);

/// @brief inicializálja a vezérlő gombokat
/// @param b a gombok tömbje
/// @param renderer a renderer
void button_ctrl_init(Button *b, SDL_Renderer* renderer);

/// @brief kattintás alapján frissíti a gombok, és a program állapotát
/// @param p a kattintás helye
/// @param b a gombok tömbje
/// @param m általános és állapotadatokat tároló struktúra
void button_event(pos p, Button *b, Master* m);


/// @brief kirendereli a gombokat
/// @param b a gombok tömbje
/// @param renderer a renderer
void button_render_all(Button *b, SDL_Renderer* renderer);

#endif