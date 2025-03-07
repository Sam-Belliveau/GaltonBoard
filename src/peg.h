#ifndef PEG_H
#define PEG_H

#include "fix15.h"
#include "vec2.h"

typedef struct {
    Vec2 position;
} Peg;

#define PEG_HORIZONTAL_SPACING 32
#define PEG_VERTICAL_SPACING 16

#define PEG_OFFSET_X 320
#define PEG_OFFSET_Y 80

#define PEG_RADIUS 4

#define NUM_ROWS 16
#define NUM_PEGS ((NUM_ROWS * (NUM_ROWS + 1)) / 2)
static Peg pegs[NUM_PEGS];

void draw_peg(Peg* peg);
Peg* get_nearest_peg(volatile Vec2* position);

void init_pegs();
void draw_pegs();

#endif // PEG_H