#include "peg.h"
#include "vga16_graphics.h"

void draw_peg(Peg *peg)
{
    if (peg->redraw)
    {
        fillCircle(peg->position.x, peg->position.y, PEG_RADIUS, WHITE);
    }
}

Peg *get_nearest_peg(Vec2 *position)
{
    static Peg nearest_peg;

    int row = ((position->y) - PEG_OFFSET_Y + (PEG_VERTICAL_SPACING / 2)) / PEG_VERTICAL_SPACING;

    if (row < 0) row = 0;
    if (row >= NUM_ROWS) row = NUM_ROWS - 1;

    Fix15 start_x = PEG_OFFSET_X - (PEG_HORIZONTAL_SPACING * row >> 1);
    int column = ((position->x) - (start_x) + (PEG_HORIZONTAL_SPACING / 2)) / PEG_HORIZONTAL_SPACING;

    if (column < 0) column = 0;
    if (column > row) column = row;

    // Compute peg index based on row/column
    nearest_peg.position.x = start_x + (PEG_HORIZONTAL_SPACING * column);
    nearest_peg.position.y = PEG_OFFSET_Y + (PEG_VERTICAL_SPACING * row);

    return &nearest_peg;
}

void init_pegs()
{
    int peg = 0;

    for (int row = 0; row < NUM_ROWS; row++)
    {
        Fix15 start_x = PEG_OFFSET_X - (PEG_HORIZONTAL_SPACING * row >> 1);
        Fix15 start_y = PEG_OFFSET_Y + (PEG_VERTICAL_SPACING * row);

        for (int column = 0; column <= row; column++)
        {
            pegs[peg].position.x = start_x + (PEG_HORIZONTAL_SPACING * column);
            pegs[peg].position.y = start_y;
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