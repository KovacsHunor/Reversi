#include "board.h"

Board board_init()
{
    return (Board){.side = BLACK, .state = BASIC, .points = {0, 0}, .valid_count = 0};
}

void board_default(Board *b)
{
    b->disks[3][4] = WHITE;
    b->disks[4][3] = WHITE;
    b->disks[3][3] = BLACK;
    b->disks[4][4] = BLACK;
    b->points[0] = 2;
    b->points[1] = 2;
}

void board_clear(Board *b)
{
    for (int y = 0; y < TILECOUNT; y++)
        for (int x = 0; x < TILECOUNT; x++)
            b->disks[y][x] = NONE;
}

Disk board_more(Board *b)
{
    if (b->points[WHITE] > b->points[BLACK])
        return WHITE;
    if (b->points[WHITE] < b->points[BLACK])
        return BLACK;
    return NONE;
}

int board_minimax(Board *b, int depth, int alpha, int beta)
{
    if (depth >= 6 || b->valid_count == 0)
    {
        return (b->points[BLACK] - b->points[WHITE]);
    }
    int best = b->side == BLACK ? INT32_MIN : INT32_MAX;
    pos bestpos;

    for (int y = 0; y < TILECOUNT; y++)
    {
        for (int x = 0; x < TILECOUNT; x++)
        {
            if (b->disks[y][x] == VALID)
            {
                Board board = *b;
                board_put_disk(&board, (pos){x, y});
                board_after_move(&board);
                int value = board_minimax(&board, depth + 1, alpha, beta);

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
        board_put_disk(b, (pos){bestpos.x, bestpos.y});
    }
    return best;
}

void board_print_state(Board *b, SDL_Renderer *renderer)
{
    pos msg = (pos){MSGX, MSGY};
    if (b->state == END)
    {
        Disk winner = board_more(b);
        if (winner == BLACK)
        {
            font_render(renderer, msg, "BLACK win");
        }
        else if (winner == WHITE)
        {
            font_render(renderer, msg, "WHITE win");
        }
        else
            font_render(renderer, msg, "DRAW");
    }
    else if (b->state == PASS)
    {
        if (b->side == BLACK)
        {
            font_render(renderer, msg, "WHITE: PASS");
        }
        else
        {
            font_render(renderer, msg, "BLACK: PASS");
        }
    }
}

Disk board_flip_color(Disk disk)
{
    if (disk == WHITE)
        return BLACK;
    if (disk == BLACK)
        return WHITE;
    return disk;
}

void board_after_move(Board *b)
{
    b->side = board_flip_color(b->side);
    board_clear_valid(b);
    board_set_valid(b);

    if (b->valid_count == 0)
    {
        b->side = board_flip_color(b->side);
        board_set_valid(b);
        if (b->valid_count == 0)
        {
            b->state = END;
        }
        else
            b->state = PASS;
    }
    else
        b->state = BASIC;
}

void board_put_disk(Board *b, pos p)
{
    board_clear_valid(b);
    b->disks[p.y][p.x] = b->side;
    board_raycast(b, (pos){p.x, p.y}, true);
}

bool board_rec_valid(Board *b, pos p, pos v, bool first, bool flip)
{
    if (p.x >= 0 && p.x < TILECOUNT && p.y >= 0 && p.y < TILECOUNT)
    {
        Disk current = b->disks[p.y][p.x];
        if (!first && current == b->side)
            return true;
        if (current == board_flip_color(b->side))
        {
            bool found = board_rec_valid(b, pos_add(p, v), v, false, flip);
            if (found && flip)
            {
                b->disks[p.y][p.x] = b->side;
                b->points[b->side]++;
                b->points[current]--;
            }
            return found;
        }
    }
    return false;
}

bool board_raycast(Board *b, pos p, bool flip)
{
    bool valid = false;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i != 0 || j != 0)
            {
                pos v = {i, j};
                valid |= board_rec_valid(b, pos_add(p, v), v, true, flip);
            }
        }
    }
    return valid;
}

void board_set_valid(Board *b)
{
    b->valid_count = 0;
    for (int y = 0; y < TILECOUNT; y++)
    {
        for (int x = 0; x < TILECOUNT; x++)
        {
            if (b->disks[y][x] == NONE && board_raycast(b, (pos){x, y}, false))
            {
                b->disks[y][x] = VALID;
                b->valid_count++;
            }
        }
    }
}

void board_render_disk(Disk d, pos p, SDL_Renderer *renderer)
{
    Image img;
    if (d == WHITE)
        img = img_init(p, "../sprites/disk_W.png", renderer, true);
    else if (d == BLACK)
        img = img_init(p, "../sprites/disk_B.png", renderer, true);
    else if (d == VALID)
        img = img_init(p, "../sprites/disk_V.png", renderer, true);
    img_render(&img, renderer);
    SDL_DestroyTexture(img.sprite);
}

void board_clear_valid(Board *b)
{
    for (int i = 0; i < TILECOUNT; i++)
    {
        for (int j = 0; j < TILECOUNT; j++)
        {
            if (b->disks[i][j] == VALID)
                b->disks[i][j] = NONE;
        }
    }
}

void board_render(SDL_Renderer *renderer, Board *b, Disk player)
{
    for (int i = 0; i < TILECOUNT; i++)
    {
        for (int j = 0; j < TILECOUNT; j++)
        {
            SDL_Rect r = {i * (TILESIZE + 1) + BOARDX, j * (TILESIZE + 1) + BOARDY, TILESIZE, TILESIZE};
            SDL_SetRenderDrawColor(renderer, 0, (i + j) % 2 ? 120 : 130, 0, 255);
            SDL_RenderFillRect(renderer, &r);

            if (b->disks[j][i] != NONE)
            {
                board_render_disk(b->disks[j][i], (pos){BOARDX + i * (TILESIZE + 1), BOARDY + j * (TILESIZE + 1)}, renderer);
            }
        }
    }

    board_print_state(b, renderer);

    if (b->points[0]+b->points[1] != 0)
    {
        char temp[50];
        sprintf(temp, "%s%d", player == WHITE ? "BLACK: " : "WHITE: ", b->points[board_flip_color(player)]);
        font_render(renderer, (pos){BOARDX + TILECOUNT * (TILESIZE + 1) / 2 - 80, BOARDY - 70}, temp);

        sprintf(temp, "%s%d", player == WHITE ? "WHITE: " : "BLACK: ", b->points[player]);
        font_render(renderer, (pos){BOARDX + TILECOUNT * (TILESIZE + 1) / 2 - 80, BOARDY + (TILESIZE + 1) * TILECOUNT + 50}, temp);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}