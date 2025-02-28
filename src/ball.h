#ifndef BALL_H
#define BALL_H

#include "fix15.h"
#include "vec2.h"
#include "peg.h"

static const Vec2 GRAVITY = {0, (Fix15)(0.75)};
static const Fix15 BOUNCE = (Fix15)(0.4);

#define BALL_RADIUS 4

typedef struct {
    Vec2 position;
    Vec2 velocity;
    char color;
    bool hit_peg;
} Ball;

// Wall detection
#define BOTTOM (PEG_OFFSET_Y + (NUM_ROWS) * (PEG_VERTICAL_SPACING))

#define hitBottom(b) ((b->position.y)>(Fix15)(BOTTOM))
#define hitTop(b) ((b->position.y)<(Fix15)(0))
#define hitLeft(b) ((b->position.x)<(Fix15)(0))
#define hitRight(b) ((b->position.x)>(Fix15)(640))

static const Vec2 SPAWN_POINT = {(Fix15)(320), (Fix15)(40)};

#define MAX_BALL_COUNT 2048

int get_ball_count();
void set_ball_count(int count);

void respawn_ball(Ball* ball);
void update_ball(Ball* ball);
void draw_ball(Ball* ball);
void clear_ball(Ball* ball);

void init_balls();
void clear_update_draw_balls();



#endif // BALL_H