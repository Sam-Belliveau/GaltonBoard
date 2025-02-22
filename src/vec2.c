#include "vec2.h"

#define abs(a) ((a) < 0 ? -(a) : (a))

void vec2_add(const Vec2* a, const Vec2* b, Vec2* result) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
}

void vec2_sub(const Vec2* a, const Vec2* b, Vec2* result) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
}

void vec2_mul(const Vec2* a, Fix15 scalar, Vec2* result) {
    result->x = multfix15(a->x, scalar);
    result->y = multfix15(a->y, scalar);
}

void vec2_div(const Vec2* a, Fix15 scalar, Vec2* result) {
    result->x = divfix(a->x, scalar);
    result->y = divfix(a->y, scalar);
}

Fix15 vec2_dot(const Vec2* a, const Vec2* b) {
    return multfix15(a->x, b->x) + multfix15(a->y, b->y);
}

Fix15 vec2_mag(const Vec2* a) {
    return sqrtfix(multfix15(a->x, a->x) + multfix15(a->y, a->y));
}

Fix15 vec2_distance(const Vec2* a, const Vec2* b) {
    Vec2 diff;
    vec2_sub(a, b, &diff);
    return vec2_mag(&diff);
}

Fix15 vec2_fast_distance(const Vec2* a, const Vec2* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

void vec2_normalize(const Vec2* a, Vec2* result) {
    Fix15 mag = vec2_mag(a);
    vec2_div(a, mag, result);
}



