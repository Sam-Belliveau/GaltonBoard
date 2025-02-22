#include "peg.h"
#include "vga16_graphics.h"

void draw_peg(Peg *peg)
{
    if (peg->redraw)
    {
        fillCircle(fix2int15(peg->position.x), fix2int15(peg->position.y), fix2int15(peg->radius), peg->color);
    }
}

Peg *get_nearest_peg(Vec2 *position)
{
    Peg *nearest = &pegs[0];
    Fix15 distance = vec2_fast_distance(&pegs[0].position, position);

    for (int i = 1; i < NUM_PEGS; i++)
    {
        Fix15 new_distance = vec2_fast_distance(&pegs[i].position, position);
        if (new_distance < distance)
        {
            nearest = &pegs[i];
            distance = new_distance;
        }
    }

    return nearest;
}
// 100 440
void init_pegs()
{
    int peg = 0;

    Fix15 HORIZONTAL_SPACING = int2fix15(38);
    Fix15 VERTICAL_SPACING = int2fix15(19);
    for (int row = 0; row < NUM_ROWS; row++)
    {
        Fix15 start_x = int2fix15(320) - (HORIZONTAL_SPACING * row >> 1);
        Fix15 start_y = int2fix15(140) + (VERTICAL_SPACING * row);

        for (int column = 0; column <= row; column++)
        {
            pegs[peg].position.x = start_x + (HORIZONTAL_SPACING * column);
            pegs[peg].position.y = start_y;
            pegs[peg].radius = int2fix15(6);
            pegs[peg].color = WHITE;
            pegs[peg].redraw = true;
            peg++;
        }
    }

    draw_pegs();
}

void draw_pegs()
{
    for (int i = 0; i < NUM_PEGS; i++)
    {
        draw_peg(&pegs[i]);
    }
}