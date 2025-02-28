#ifndef FIX_15_h
#define FIX_15_h

#include "pico/divider.h"
#include <math.h>

typedef _Accum Fix15 ;

#define sqrtfix(a) ((Fix15)sqrtf((float)(a)))

#endif // FIX_15_h