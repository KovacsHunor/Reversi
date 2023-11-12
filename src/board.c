#include "board.h"

Board board_init(int board_length, Master *m)
{
    return (Board){.msg = (pos){200, 500}, .side = {2, 2}, .valid_count = 0, .length = board_length, .tile_count = TILECOUNT, .tile_size = board_length / TILECOUNT, .x = m->width / 2 - board_length / 2, .y = m->height / 2 - board_length / 2};
}

void board_default(Board *b, Master *m)
{
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            b->disks[i][j] = board_create_disk(b->x + i * (b->tile_size + 1), b->y + j * (b->tile_size + 1), NONE, m);
        }
    }
    board_set_color(&b->disks[3][4], WHITE, m);
    board_set_color(&b->disks[4][3], WHITE, m);
    board_set_color(&b->disks[3][3], BLACK, m);
    board_set_color(&b->disks[4][4], BLACK, m);
}

disk_color board_more(Board *b)
{
    int black = 0;
    int white = 0;
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            if (b->disks[i][j].color == BLACK)
                black++;
            else if (b->disks[i][j].color == WHITE)
                white++;
        }
    }
    if (black > white)
        return BLACK;
    if (white > black)
        return WHITE;
    return NONE;
}

int minimax(Board *b, int depth, int alpha, int beta, bool maximizing, Master *m)
{
    if (depth >= 3 || b->valid_count == 0)
    {
        return b->side[BLACK] - b->side[WHITE];
    }
    int best = maximizing ? INT32_MIN : INT32_MAX;
    pos bestpos;
    disk_color side = maximizing ? BLACK : WHITE;

    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            if (b->disks[i][j].color == VALID)
            {
                Board board = *b;
                board_clear(&board, m, true);
                board_set_color(&board.disks[i][j], side, m);
                board_raycast(&board, (pos){j, i}, side, true, m);
                board_set_valid(&board, board_flip_color(side), m);
                int value = minimax(&board, depth + 1, alpha, beta, !maximizing, m);

                if (maximizing)
                {
                    if (best < value)
                    {
                        best = value;
                        bestpos = (pos){j, i};
                    }
                    alpha = max(alpha, best);
                    if (beta <= alpha)
                        return best;
                }
                else
                {
                    if (best > value)
                    {
                        best = value;
                        bestpos = (pos){j, i};
                    }
                    beta = min(beta, best);
                    if (beta <= alpha)
                        return best;
                }
            }
        }
    }
    if (depth == 0)
    {
        b->side[side]++;
        board_clear(b, m, true);
        board_set_color(&b->disks[bestpos.y][bestpos.x], side, m);
        board_raycast(b, bestpos, side, true, m);
    }
    return best;
}

void board_print_event(Board *b, Master *m, disk_color *side){
    if (b->valid_count == 0)
    {
        *side = board_flip_color(*side);
        board_set_valid(b, *side, m);
        if (b->valid_count == 0)
        {
            *side = board_flip_color(*side);
            disk_color winner = board_more(b);
            if (winner == BLACK)
            {
                font_render(m, b->msg, "BLACK win");
            }
            else if (winner == WHITE)
            {
                font_render(m, b->msg, "WHITE win");
            }
            else
                font_render(m, b->msg, "DRAW");
            
        }
        else
        {
            if (*side == BLACK)
            {
                font_render(m, b->msg, "WHITE: PASS");
            }
            else
            {
                font_render(m, b->msg, "BLACK: PASS");
            }
        }
    }
}

void board_event(Board *b, int x, int y, disk_color *side, Master *m)
{
    if (*side == BLACK)
    {
        if (!pos_hover((pos){b->x, b->y}, (pos){b->length, b->length}, (pos){x, y}))
        {
            board_render(m, b);
            return;
        }

        int i = (x - b->x) / (b->tile_size + 1);
        int j = (y - b->y) / (b->tile_size + 1);

        if (b->disks[i][j].color == VALID && img_hover(&b->disks[i][j].img, x, y))
        {
            b->side[*side]++;
            board_clear(b, m, true);
            board_set_color(&b->disks[i][j], *side, m);
            board_raycast(b, (pos){j, i}, *side, true, m);
            *side = board_flip_color(*side);
            board_set_valid(b, *side, m);
        }
    }
    else
    {
        board_render(m, b);
        SDL_RenderPresent(m->renderer);

        minimax(b, 0, INT32_MIN, INT32_MAX, false, m);
        *side = board_flip_color(*side);
        board_set_valid(b, *side, m);
    }

    board_print_event(b, m, side);

    if (*side == WHITE)
        board_event(b, x, y, side, m);
    board_render(m, b);
}

disk_color board_flip_color(disk_color c)
{
    if (c == WHITE)
        return BLACK;
    if (c == BLACK)
        return WHITE;
    return c;
}

bool board_rec_valid(Board *b, pos p, pos v, disk_color c, bool first, bool flip, Master *m)
{
    if (p.x >= 0 && p.x < b->tile_count && p.y >= 0 && p.y < b->tile_count)
    {
        disk_color current = b->disks[p.y][p.x].color;
        if (!first && current == c)
            return true;
        if (current == board_flip_color(c))
        {
            bool found = board_rec_valid(b, pos_add(p, v), v, c, false, flip, m);
            if (found && flip)
            {
                board_set_color(&b->disks[p.y][p.x], c, m);
                b->side[c]++;
                b->side[current]--;
            }
            return found;
        }
    }
    return false;
}

bool board_raycast(Board *b, pos p, disk_color c, bool flip, Master *m)
{
    bool valid = false;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i != 0 || j != 0)
            {
                pos v = {i, j};
                valid |= board_rec_valid(b, pos_add(p, v), v, c, true, flip, m);
            }
        }
    }
    return valid;
}

void board_set_valid(Board *b, disk_color c, Master *m)
{
    b->valid_count = 0;
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            if (b->disks[i][j].color == NONE && board_raycast(b, (pos){j, i}, c, false, m))
            {
                board_set_color(&b->disks[i][j], VALID, m);
                b->valid_count++;
            }
        }
    }
}

void board_set_color(Disk *d, disk_color c, Master *m)
{
    d->color = c;
    if (c == WHITE)
    {
        d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/disk_W.png");
        return;
    }
    if (c == BLACK)
    {
        d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/disk_B.png");
        return;
    }
    if (c == VALID)
    {
        d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/disk_V.png");
        return;
    }
    // d->img.sprite = IMG_LoadTexture(m->renderer, "../sprites/none.png");
}

void board_clear(Board *b, Master *m, bool only_valid)
{
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            if (only_valid && b->disks[i][j].color != VALID)
                continue;
            board_set_color(&b->disks[i][j], NONE, m);
        }
    }
}

Disk board_create_disk(int x, int y, disk_color c, Master *m)
{
    Disk d = {c, img_init(x, y, "../sprites/disk_B.png", m, true)};
    board_set_color(&d, c, m);
    return d;
}

void board_render(Master *m, Board *b)
{
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            SDL_Rect r = {i * (b->tile_size + 1) + b->x, j * (b->tile_size + 1) + b->y, b->tile_size, b->tile_size};
            SDL_SetRenderDrawColor(m->renderer, 0, (i + j) % 2 ? 120 : 130, 0, 255);
            SDL_RenderFillRect(m->renderer, &r);

            if (b->disks[i][j].color != NONE)
            {
                img_render(&b->disks[i][j].img, m);
            }
        }
    }
    char temp[50];
    sprintf(temp, "WHITE: %d", b->side[WHITE]);
    font_render(m, (pos){b->x + b->length / 2, b->y - 100}, temp);

    sprintf(temp, "BLACK: %d", b->side[BLACK]);
    font_render(m, (pos){b->x + b->length / 2, b->y + b->length + 100}, temp);

    SDL_SetRenderDrawColor(m->renderer, 0, 0, 0, 255);
}