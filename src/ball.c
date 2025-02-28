#include "ball.h"
#include "dma.h"
#include "vga16_graphics.h"

#include <stdlib.h>

static Vec2 random_velocity()
{
  return (Vec2){
      (rand() & 0x1fff) - 0xfff,
      (rand() & 0x1fff) - 0xfff};
}

void respawn_ball(Ball *ball)
{
  ball->position = SPAWN_POINT;
  ball->velocity = random_velocity();
  ball->radius = (Fix15)(4);
  ball->color = 1 | (rand() & 0b1111);
}

void update_ball(Ball *ball)
{
  Peg *nearest_peg = get_nearest_peg(&ball->position);

  Vec2 difference;
  vec2_sub(&ball->position, &nearest_peg->position, &difference);

  Fix15 collision_distance = ball->radius + nearest_peg->radius;

  if (abs(difference.x) < collision_distance && abs(difference.y) < collision_distance)
  {
    nearest_peg->redraw = true;

    Fix15 distance = vec2_mag(&difference);

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
  fillCircle(ball->position.x, ball->position.y, ball->radius, ball->color);
}

void clear_ball(Ball *ball)
{
  fillCircle(ball->position.x, ball->position.y, ball->radius, BLACK);
}

void init_balls()
{
  for (int i = 0; i < NUM_BALLS; i++)
  {
    respawn_ball(&balls[i]);
  }
}

void update_balls()
{
  for (int i = 0; i < NUM_BALLS; i++)
  {
    update_ball(&balls[i]);
  }
}

void draw_balls()
{
  for (int i = 0; i < NUM_BALLS; i++)
  {
    draw_ball(&balls[i]);
  }
}
void clear_balls()
{
  for (int i = 0; i < NUM_BALLS; i++)
  {
    clear_ball(&balls[i]);
  }
}