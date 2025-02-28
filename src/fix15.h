#ifndef FIX_15_h
#define FIX_15_h

#include "pico/divider.h"
#include <math.h>

typedef _Accum Fix15 ;

#define sqrtfix(a) ((Fix15)sqrtf((float)(a)))

/*
inline static Fix15 fast_sqrt(Fix15 a) {
    const int shift = (17 - __builtin_clz(a)) >> 1;
    
    Fix15 result = (0 < shift) ? a >> shift : a << -shift;

    return result;
}
*/

static inline Fix15 fast_fix_div(Fix15 a, Fix15 b) {
    const int32_t a_raw = *(int32_t*)&a;
    const int32_t b_raw = *(int32_t*)&b;

    const int32_t result = div_s64s64(((int64_t)a_raw) << 15, (int64_t)b_raw);
    return *(Fix15*)&result;
}


static const Fix15 sqrt2 = 1.41421356237309504880;

static inline Fix15 fast_sqrt(Fix15 a) {
    const uint32_t a_raw = *(uint32_t*)&a;
    const int shift = (17 - __builtin_clz(a_raw)) >> 1;

    Fix15 x = (shift > 0) ? a >> shift : a << (-shift);
    x = (x + fast_fix_div(a, x)) >> 1;
    x = (x + fast_fix_div(a, x)) >> 1;
    x = (x + fast_fix_div(a, x)) >> 1;

    return x;
}


#endif // FIX_15_h