#ifndef DRAW_STATS
#define DRAW_STATS

#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"

void draw_hist(int *counts);
void update_ball_count();

void normalize_hist();


#endif DRAW_STATS