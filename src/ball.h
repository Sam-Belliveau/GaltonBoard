#ifndef BALL_H
#define BALL_H

#include "fix15.h"
#include "vec2.h"
#include "peg.h"

static const Vec2 GRAVITY = {0, (Fix15)(0.75)};
extern Fix15 BOUNCE;

#define BALL_RADIUS 2

//Struct for all balls
//Positions all stored as x,y form in Vec2 type
typedef struct {
    Vec2 position; 
    Vec2 last_drawn;
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

// Coordinate from where to create and drop balls
static const Vec2 SPAWN_POINT = {(Fix15)(320), (Fix15)(40)};

#define MAX_BALL_COUNT 3600

//Count variables needed for stats display
extern int previous_ball_count;
extern int ball_count;

//Global Ball array
extern Ball balls[MAX_BALL_COUNT];

int get_ball_count(); // Get ball_count being currently simulated 
void set_ball_count(int count); //Set ball_count

//Individual Ball Functions 
void respawn_ball(Ball* ball); 
void update_ball(Ball* ball);
void draw_ball(Ball* ball);
void clear_ball(Ball* ball);

//By frame functions for all balls
void init_balls(); 
void clear_draw_balls();
void update_balls();



#endif // BALL_H
