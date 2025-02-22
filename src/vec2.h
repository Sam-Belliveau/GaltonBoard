#ifndef VEC2_H
#define VEC2_H

#include "fix15.h"

typedef struct {
    Fix15 x;
    Fix15 y;
} Vec2;

void vec2_add(const Vec2* a, const Vec2* b, Vec2* result);
void vec2_sub(const Vec2* a, const Vec2* b, Vec2* result);
void vec2_mul(const Vec2* a, Fix15 scalar, Vec2* result);
void vec2_div(const Vec2* a, Fix15 scalar, Vec2* result);

Fix15 vec2_dot(const Vec2* a, const Vec2* b);
Fix15 vec2_mag(const Vec2* a);
Fix15 vec2_distance(const Vec2* a, const Vec2* b);
Fix15 vec2_fast_distance(const Vec2* a, const Vec2* b);

void vec2_normalize(const Vec2* a, Vec2* result);

#endif // VEC2_H