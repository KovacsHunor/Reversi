#include "board.h"

Board board_init(int board_length, Master *m)
{
    return (Board){.side = BLACK, .state = BASIC, .msg = (pos){200, 500}, .points = {2, 2}, .valid_count = 0, .length = board_length, .tile_count = TILECOUNT, .tile_size = board_length / TILECOUNT, .position = (pos){m->width / 2 - board_length / 2, m->height / 2 - board_length / 2}};
}

void board_default(Board *b, Master *m)
{
    for (int y = 0; y < b->tile_count; y++)
    {
        for (int x = 0; x < b->tile_count; x++)
        {
            b->disks[y][x] = board_create_disk(b->position.x + x * (b->tile_size + 1), b->position.y + y * (b->tile_size + 1), NONE, m);
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

int minimax(Board *b, int depth, int alpha, int beta, Master *m)
{
    if (depth >= 6 || b->valid_count == 0)
    {
        return (b->points[BLACK] - b->points[WHITE]);
    }
    int best = b->side == BLACK ? INT32_MIN : INT32_MAX;
    pos bestpos;

    for (int y = 0; y < b->tile_count; y++)
    {
        for (int x = 0; x < b->tile_count; x++)
        {
            if (b->disks[y][x].color == VALID)
            {
                Board board = *b;
                board_put_disk(&board, (pos){x, y}, m);
                board_after_move(&board, m);
                int value = minimax(&board, depth + 1, alpha, beta, m);

                if (b->side == BLACK)
                {
                    if (best < value)
                    {
                        best = value;
                        bestpos = (pos){x, y};
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
                        bestpos = (pos){x, y};
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
        b->points[b->side]++;
        board_put_disk(b, (pos){bestpos.x, bestpos.y}, m);
    }
    return best;
}

void board_print_event(Board *b, Master *m)
{
    if (b->state == END)
    {
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
    else if (b->state == PASS)
    {
        if (b->side == BLACK)
        {
            font_render(m, b->msg, "WHITE: PASS");
        }
        else
        {
            font_render(m, b->msg, "BLACK: PASS");
        }
    }
}

disk_color board_flip_color(disk_color c)
{
    if (c == WHITE)
        return BLACK;
    if (c == BLACK)
        return WHITE;
    return c;
}

void board_after_move(Board *b, Master *m)
{
    b->side = board_flip_color(b->side);
    board_clear(b, m, true);
    board_set_valid(b, m);

    if (b->valid_count == 0)
    {
        b->side = board_flip_color(b->side);
        board_set_valid(b, m);
        if (b->valid_count == 0){
            b->state = END;
        }
        else b->state = PASS;
    }
    else b->state = BASIC;
}

void board_put_disk(Board *b, pos p, Master *m)
{
    board_clear(b, m, true);
    board_set_color(&b->disks[p.y][p.x], b->side, m);
    board_raycast(b, (pos){p.x, p.y}, true, m);
}

bool board_rec_valid(Board *b, pos p, pos v, bool first, bool flip, Master *m)
{
    if (p.x >= 0 && p.x < b->tile_count && p.y >= 0 && p.y < b->tile_count)
    {
        disk_color current = b->disks[p.y][p.x].color;
        if (!first && current == b->side)
            return true;
        if (current == board_flip_color(b->side))
        {
            bool found = board_rec_valid(b, pos_add(p, v), v, false, flip, m);
            if (found && flip)
            {
                board_set_color(&b->disks[p.y][p.x], b->side, m);
                b->points[b->side]++;
                b->points[current]--;
            }
            return found;
        }
    }
    return false;
}

bool board_raycast(Board *b, pos p, bool flip, Master *m)
{
    bool valid = false;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i != 0 || j != 0)
            {
                pos v = {i, j};
                valid |= board_rec_valid(b, pos_add(p, v), v, true, flip, m);
            }
        }
    }
    return valid;
}

void board_set_valid(Board *b, Master *m)
{
    b->valid_count = 0;
    for (int y = 0; y < b->tile_count; y++)
    {
        for (int x = 0; x < b->tile_count; x++)
        {
            if (b->disks[y][x].color == NONE && board_raycast(b, (pos){x, y}, false, m))
            {
                board_set_color(&b->disks[y][x], VALID, m);
                b->valid_count++;
            }
        }
    }
}

void board_render_disk(Disk *d, pos p, Master *m)
{
    Image img;
    if (d->color == WHITE)
        img = img_init(p.x, p.y, "../sprites/disk_W.png", m, true);
    else if (d->color == BLACK)
        img = img_init(p.x, p.y, "../sprites/disk_B.png", m, true);
    else if (d->color == VALID)
        img = img_init(p.x, p.y, "../sprites/disk_V.png", m, true);
    img_render(&img, m);
    SDL_DestroyTexture(img.sprite);
}

void board_set_color(Disk *d, disk_color c, Master *m)
{
    d->color = c;
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
    Disk d = {c, (pos){x, y}};
    board_set_color(&d, c, m);
    return d;
}

void board_render(Master *m, Board *b)
{
    for (int i = 0; i < b->tile_count; i++)
    {
        for (int j = 0; j < b->tile_count; j++)
        {
            SDL_Rect r = {i * (b->tile_size + 1) + b->position.x, j * (b->tile_size + 1) + b->position.y, b->tile_size, b->tile_size};
            SDL_SetRenderDrawColor(m->renderer, 0, (i + j) % 2 ? 120 : 130, 0, 255);
            SDL_RenderFillRect(m->renderer, &r);

            if (b->disks[j][i].color != NONE)
            {
                board_render_disk(&b->disks[j][i], b->disks[j][i].p, m);
            }
        }
    }

    board_print_event(b, m);

    char temp[50];
    sprintf(temp, "WHITE: %d", b->points[WHITE]);
    font_render(m, (pos){b->position.x + b->length / 2, b->position.y - 100}, temp);

    sprintf(temp, "BLACK: %d", b->points[BLACK]);
    font_render(m, (pos){b->position.x + b->length / 2, b->position.y + b->length + 50}, temp);

    SDL_SetRenderDrawColor(m->renderer, 0, 0, 0, 255);
}