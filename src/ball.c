#include "ball.h"
#include "dma.h"
#include "draw_stats.h"
#include "vga16_graphics.h"

#include <stdlib.h>

int previous_ball_count = 0;
int ball_count = 16;
Ball balls[MAX_BALL_COUNT];

Fix15 BOUNCE = (Fix15)(0.4);

static Vec2 random_velocity()
{
  return (Vec2){
      (Fix15)((rand() & 0x7fff) - 0x3fff) >> 15,
      (Fix15)((rand() & 0x7fff) - 0x3fff) >> 15};
}

int get_ball_count()
{
  return ball_count;
}

void set_ball_count(int count)
{
  if (count > MAX_BALL_COUNT)
    count = MAX_BALL_COUNT;
  else if (count < 0)
    count = 0;

  ball_count = count;
}

void respawn_ball(Ball *ball)
{
  ball->position = SPAWN_POINT;
  ball->velocity = random_velocity();
  ball->color = 1 | (rand() & 0b1111);
}

void update_ball(Ball *ball)
{
  const Peg *nearest_peg = get_nearest_peg(&ball->position);

  Vec2 difference;
  vec2_sub(&ball->position, &nearest_peg->position, &difference);

  const Fix15 collision_distance = BALL_RADIUS + PEG_RADIUS;
  Fix15 distance = vec2_mag(&difference);
  
  if (distance <= collision_distance)
  {
    Vec2 normal;
    vec2_div(&difference, distance, &normal);

    Fix15 intermediate = -2 * vec2_dot(&ball->velocity, &normal);

    if (intermediate > 0)
    {
      Vec2 new_offset;
      vec2_mul(&normal, collision_distance + 1, &new_offset);
      vec2_add(&nearest_peg->position, &new_offset, &ball->position);

      vec2_mul(&normal, intermediate, &normal);
      vec2_add(&ball->velocity, &normal, &ball->velocity);

      vec2_mul(&ball->velocity, BOUNCE, &ball->velocity);

      if (!ball->hit_peg)
      {
        start_audio();
      }

      ball->hit_peg = true;
    }
  }
  else
  {
    ball->hit_peg = false;
  }

  if (hitBottom(ball))
  {
    int bin = get_bin(ball->position.x);
    count_ball(bin);

    respawn_ball(ball);
  }

  else if (hitLeft(ball))
  {
    ball->velocity.x = abs(ball->velocity.x);
  }

  else if (hitRight(ball))
  {
    ball->velocity.x = -abs(ball->velocity.x);
  }

  else if (hitTop(ball))
  {
    ball->velocity.y = -ball->velocity.y;
  }

  vec2_add(&ball->velocity, &GRAVITY, &ball->velocity);
  vec2_add(&ball->position, &ball->velocity, &ball->position);
}

void draw_ball(Ball *ball)
{
  drawCircle(ball->position.x, ball->position.y, BALL_RADIUS, ball->color);
  ball->last_drawn = ball->position;
}

void clear_ball(Ball *ball)
{
  drawCircle(ball->last_drawn.x, ball->last_drawn.y, BALL_RADIUS, BLACK);
}

void init_balls()
{
  for (int i = 0; i < ball_count; i++)
  {
    respawn_ball(&balls[i]);
  }
}

void clear_draw_balls()
{
  for (int i = 0; i < ball_count; i++)
  {
    clear_ball(&balls[i]);
    draw_ball(&balls[i]);
  }

  if (ball_count < previous_ball_count)
  {
    for (int i = ball_count; i < previous_ball_count; i++)
    {
      clear_ball(&balls[i]);
    }
  }
  else if (ball_count > previous_ball_count)
  {
    for (int i = previous_ball_count; i < ball_count; i++)
    {
      clear_ball(&balls[i]);
      respawn_ball(&balls[i]);
    }
  }

  previous_ball_count = ball_count;
}

void update_balls()
{
  for (int i = 0; i < ball_count; i++)
  {
    update_ball(&balls[i]);
  }
}