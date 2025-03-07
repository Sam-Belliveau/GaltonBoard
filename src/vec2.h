#ifndef VEC2_H
#define VEC2_H

#include "fix15.h"

// Define the Vec2 structure
typedef struct
{
    Fix15 x;
    Fix15 y;
} Vec2;

static inline void vec2_add(const Vec2 *a, const Vec2 *b, Vec2 *result)
{
    result->x = a->x + b->x;
    result->y = a->y + b->y;
}

static inline void vec2_sub(const Vec2 *a, const Vec2 *b, Vec2 *result)
{
    result->x = a->x - b->x;
    result->y = a->y - b->y;
}

static inline void vec2_mul(const Vec2 *a, Fix15 scalar, Vec2 *result)
{
    result->x = a->x * scalar;
    result->y = a->y * scalar;
}

static inline void vec2_div(const Vec2 *a, Fix15 scalar, Vec2 *result)
{
    result->x = fast_fix_div(a->x, scalar);
    result->y = fast_fix_div(a->y, scalar);
}

static inline Fix15 vec2_dot(const Vec2 *a, const Vec2 *b)
{
    return a->x * b->x + a->y * b->y;
}

static inline Fix15 vec2_mag(const Vec2 *x)
{
    const static Fix15 a0 = 0.960433870103k;
    const static Fix15 b0 = 0.397824734759k;

    // Fast approximation of the absolute value
    const Fix15 a = Fix15_abs(x->x);
    const Fix15 b = Fix15_abs(x->y);

    if (a < b)
        return b0 * a + a0 * b;
    else
        return a0 * a + b0 * b;
}

static inline Fix15 vec2_distance(const Vec2 *a, const Vec2 *b)
{
    Vec2 diff;
    vec2_sub(a, b, &diff);
    return vec2_mag(&diff);
}

static inline void vec2_normalize(const Vec2 *a, Vec2 *result)
{
    Fix15 mag = vec2_mag(a);
    vec2_div(a, mag, result);
}

#endif // VEC2_H
