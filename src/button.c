#include "button.h"

void button_ctrl_init(Button* b, Master* m){
    b[MENU] = (Button){img_init(m->width-200, 100, "../sprites/menu_B.png", m, true), IMG_LoadTexture(m->renderer, "../sprites/menu_A.png"), false};
    b[PLAY] = (Button){img_init(m->width-200, 300, "../sprites/play_B.png", m, true), IMG_LoadTexture(m->renderer, "../sprites/play_A.png"), false};
    b[HISTORY] = (Button){img_init(m->width-200, 500, "../sprites/history_B.png", m, true), IMG_LoadTexture(m->renderer, "../sprites/history_A.png"), false};
}

void button_press(Button* b, button_id id){
    img_texture_swap(&b[id].sw_sprite, &b[id].img.sprite);
    b[id].pressed = true;
}

void button_ctrl_default(Button* b){
    for (int i = 1; i < SIZE; i++)
    {
        if(b[i].pressed){
            img_texture_swap(&b[i].sw_sprite, &b[i].img.sprite);
            b[i].pressed = false;
        }
    }
}


bool button_pressable(Button* b, button_id id, int x, int y){
    return b[id].img.visible && !b[id].pressed && img_hover(&b[id].img, x, y);
}

void button_event(int x, int y, Button* b, Master* m){
    if (img_hover(&b[MENU].img, x, y)){

        for (int i = 1; i < SIZE; i++)
        {
            b[i].img.visible = !b[i].img.visible;
        }
        img_texture_swap(&b[MENU].sw_sprite, &b[MENU].img.sprite);
        b[MENU].pressed = !b[MENU].pressed;
    }
    if (button_pressable(b, PLAY, x, y)){

        button_ctrl_default(b);
        button_press(b, PLAY);
    }
    if (button_pressable(b, HISTORY, x, y)){

        button_ctrl_default(b);
        button_press(b, HISTORY);
    }
    button_render_all(b, m);
}

void button_render_all(Button* b, Master* m){
    for (int i = 0; i < SIZE; i++)
    {
        if(b[i].img.visible){
            img_render(&b[i].img, m);
        }
    }
}