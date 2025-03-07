#ifndef FIX_15_h
#define FIX_15_h

#include "pico/divider.h"
#include <math.h>

typedef _Accum Fix15 ;

static inline Fix15 fast_fix_div(Fix15 a, Fix15 b) {
    const int32_t a_raw = *(int32_t*)&a;
    const int32_t b_raw = *(int32_t*)&b;

    const int32_t result = div_s64s64(((int64_t)a_raw) << 15, (int64_t)b_raw);
    return *(Fix15*)&result;
}

#define Fix15_abs(x) ((x) < 0 ? -(x) : (x))

#endif // FIX_15_h