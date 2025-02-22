#ifndef FIX_15_h
#define FIX_15_h

#include "pico/divider.h"
#include <math.h>

typedef signed int Fix15 ;
#define multfix15(a,b) ((Fix15)((((signed long long)(a))*((signed long long)(b)))>>15))
#define float2fix15(a) ((Fix15)((a)*32768.0)) // 2^15
#define fix2float15(a) ((float)(a)/32768.0)
#define absfix15(a) abs(a) 
#define int2fix15(a) ((Fix15)(a << 15))
#define fix2int15(a) ((int)(a >> 15))
#define char2fix15(a) (Fix15)(((Fix15)(a)) << 15)
#define divfix(a,b) (Fix15)(div_s64s64( (((signed long long)(a)) << 15), ((signed long long)(b))))
#define sqrtfix(a) (float2fix15(sqrt(fix2float15(a))))

#endif // FIX_15_h