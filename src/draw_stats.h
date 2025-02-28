#ifndef DRAW_STATS
#define DRAW_STATS

#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"

#include "fix15.h"
#include "peg.h"

#define NUM_BINS (NUM_ROWS + 1)

static int TOTAL_COUNT = 0;
static int time_since_start = 0;

static int MAX_COUNT = 0;
static int COUNTS[NUM_BINS];

static int PREVIOUS_MAX = 1;
static int PREVIOUS_COUNTS[NUM_BINS];

#define BIN_WIDTH (((Fix15)640) / NUM_BINS)

void reset_counts();

int get_bin(Fix15 x_position);
void count_ball(int bin);

void draw_stats(uint32_t time_since_boot_us);
void draw_histogram();

#endif // DRAW_STATS