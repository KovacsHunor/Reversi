#include "board.h"
#define TILECOUNT 8

Board board_init(int board_length, Master* m){
    return (Board){.tile_count = TILECOUNT, .tile_size = board_length/TILECOUNT, .x = m->width/2 - board_length/2, .y = m->height/2 - board_length/2};
}

void board_disks_init(Board* b){
    b->disks = (Disk**)malloc(b->tile_count*sizeof(Disk*));
    for (int i = 0; i < b->tile_count; i++)
    {
        b->disks[i] = (Disk*)malloc(b->tile_count*sizeof(Disk));
    }
}

void board_default(Board* b, Master* m){
    for (int i = 0; i < b->tile_count; i++){
        for (int j = 0; j < b->tile_count; j++)
        {
            b->disks[i][j] = board_create_disk(b->x + i*(b->tile_size + 1), b->y + j*(b->tile_size + 1), 3<=i&&i<=4&&3<=j&&j<=4 ? true : false, BLACK, m);
        }
    }
    board_set_color(&b->disks[3][4], WHITE, m);
    board_set_color(&b->disks[4][3], WHITE, m);
}

void board_destruct(Board* b){
    for (int i = 0; i < b->tile_count; i++)
    {
        free(b->disks[i]);
    }
    free(b->disks);
}

disk_color board_flip_color(disk_color c){
    if(c == WHITE) return BLACK;
    if(c == BLACK) return WHITE;
    return c;
}

bool board_rec_valid(Board* b, pos p, pos v, disk_color c, bool first){
    if(p.x>=0 && p.x < b->tile_count && p.y>=0 && p.y<b->tile_count && b->disks[p.y][p.x].img.visible){
        disk_color current = b->disks[p.y][p.x].color;
        if((first && current != board_flip_color(c))) return false;
        
        if(current == c) return true;
        if(current == board_flip_color(c)) return board_rec_valid(b, pos_add(p, v), v, c, false);
    }
    return false;
}

bool board_validate(Board* b, pos p, disk_color c){
    bool valid = false;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if(i != 0 || j != 0){
                pos v = {i, j};
                valid |= board_rec_valid(b, pos_add(p, v), v, c, true);
            }
        }
    }
    return valid;
}

void board_set_valid(Board* b, disk_color c, Master* m){
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            if(board_validate(b, (pos){i, j}, c)){
                board_set_color(&b->disks[i][j], VALID, m);
                b->disks[i][j].img.visible = true;
            }
            printf("%d ", b->disks[i][j].color);
        }
        printf("\n");
    }
}

void board_set_color(Disk* d, disk_color c, Master* m){
    d->color = c;
    if(c == WHITE){ d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/disk_W.png"); return;}
    if(c == BLACK){ d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/disk_B.png"); return;}
    if(c == VALID){ d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/disk_V.png");}
}

void board_clear_board(Board* b){
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            b->disks[i][j].img.visible = false;
        }
    }
}

Disk board_create_disk(int x, int y, bool visible, disk_color color, Master* m)
{
    Disk d = {BLACK, img_init(x, y, "../sprites/disk_B.png", m, visible)};
    if(color == WHITE) board_set_color(&d, color, m);
    return d;
};

void board_render(Master* m, Board* b)
{
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            SDL_Rect r = {i * (b->tile_size + 1) + b->x, j * (b->tile_size + 1) + b->y, b->tile_size, b->tile_size};
            SDL_SetRenderDrawColor(m->renderer, 0, (i + j) % 2 ? 120 : 130, 0, 255);
            SDL_RenderFillRect(m->renderer, &r);

            if(b->disks[i][j].img.visible){
                img_render(&b->disks[i][j].img, m);
            }
        }
    }
    SDL_SetRenderDrawColor(m->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(m->renderer);
}