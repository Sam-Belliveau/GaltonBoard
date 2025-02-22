#ifndef PEG_H
#define PEG_H

#include "fix15.h"
#include "vec2.h"

typedef struct {
    Vec2 position;
    Fix15 radius;
    char color;
    bool redraw;
} Peg;

#define NUM_ROWS 10
#define NUM_PEGS ((NUM_ROWS * (NUM_ROWS + 1)) / 2)
static Peg pegs[NUM_PEGS];


void draw_peg(Peg* peg);
Peg* get_nearest_peg(Vec2* position);

void init_pegs();
void draw_pegs();

#endif // PEG_H