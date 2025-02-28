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

inline static Fix15 fast_sqrt(Fix15 a) {
    // Handle non-positive input gracefully.
    if (a <= 0)
        return 0;
    
    // Compute an initial guess.
    // __builtin_clz expects an unsigned value.
    int shift = (17 - __builtin_clz((unsigned int)a)) >> 1;
    Fix15 x = (shift > 0) ? a >> shift : a << (-shift);
    
    // // Refine the guess with a few iterations.
    // // Newton's iteration: x = (x + a / x) / 2.
    // for (int i = 0; i < 3; i++) {
    //     Fix15 x_div = fast_fix_div(a, x);  // fixed-point division: a / x
    //     x = (x + x_div) >> 1;                // average the guess with a/x
    // }
    
    return x;
}

static inline Fix15 fast_fix_div(Fix15 a, Fix15 b) {
    const int32_t a_raw = *(int32_t*)&a;
    const int32_t b_raw = *(int32_t*)&b;

    const int32_t result = div_s64s64(((int64_t)a_raw) << 15, (int64_t)b_raw);
    return *(Fix15*)&result;
}

#endif // FIX_15_h