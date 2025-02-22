#ifndef BALL_H
#define BALL_H

#include "fix15.h"
#include "vec2.h"
#include "peg.h"

static const Vec2 GRAVITY = {0, float2fix15(0.5)};
static const Fix15 BOUNCE = float2fix15(0.75);

typedef struct {
    Vec2 position;
    Vec2 velocity;
    Fix15 radius;
    char color;
    bool hit_peg;
} Ball;

// Wall detection
#define hitBottom(b) ((b->position.y)>int2fix15(380))
#define hitTop(b) ((b->position.y)<int2fix15(100))
#define hitLeft(b) ((b->position.x)<int2fix15(100))
#define hitRight(b) ((b->position.x)>int2fix15(540))

static const Vec2 SPAWN_POINT = {int2fix15(320), int2fix15(120)};

#define NUM_BALLS 16
static Ball balls[NUM_BALLS];

void respawn_ball(Ball* ball);
void update_ball(Ball* ball);
void draw_ball(Ball* ball);
void clear_ball(Ball* ball);

void init_balls();
void update_balls();
void draw_balls();
void clear_balls();

#endif // BALL_H