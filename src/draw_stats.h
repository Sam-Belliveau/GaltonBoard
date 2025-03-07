#ifndef DRAW_STATS
#define DRAW_STATS

#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"

#include "fix15.h"
#include "peg.h"

#define NUM_BINS (NUM_ROWS + 1)

extern int TOTAL_COUNT;
extern int time_since_start;

extern int MAX_COUNT;
extern int COUNTS[NUM_BINS];

extern int PREVIOUS_MAX;
extern int PREVIOUS_COUNTS[NUM_BINS];

#define BIN_WIDTH (((Fix15)640) / NUM_BINS)

void reset_counts();

int get_bin(Fix15 x_position);
void count_ball(int bin);

void draw_stats(int32_t time_since_boot_us, int32_t physics_us, int32_t draw_us, int state);
void draw_histogram();

#endif // DRAW_STATS