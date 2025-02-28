#include "draw_stats.h"

#include <stdio.h>

#include "vga16_graphics.h"
#include "ball.h"


void reset_counts() {
    for (int i = 0; i < NUM_BINS; i++) {
        COUNTS[i] = 0;
        PREVIOUS_COUNTS[i] = 0;
    }

    TOTAL_COUNT = 0;
    PREVIOUS_MAX = 1;
}

int get_bin(Fix15 x_position)
{
    return fast_fix_div(
        x_position,
        BIN_WIDTH);
}

void count_ball(int bin)
{
    if (bin < 0) {
        bin = 0;
    }

    if (bin >= NUM_BINS) {
        bin = NUM_BINS - 1;
    }

    TOTAL_COUNT += 1;
    COUNTS[bin]++;

    if (++COUNTS[bin] > MAX_COUNT) {
        MAX_COUNT = COUNTS[bin];
    }
}

void draw_stats(uint32_t time_since_boot_us)
{
    setCursor(0, 0);
    setTextSize(2);
    setTextColor2(GREEN, BLACK);
 
    char ball_count_buffer[256];
    sprintf(ball_count_buffer, "Time: %ds        \nTotal Balls: %d         \nCurrent Balls: %d        \n", time_since_boot_us / 1000000, TOTAL_COUNT, get_ball_count());
    writeString(ball_count_buffer);
}

void draw_histogram() {
    
    Fix15 max_bin_height = 480 - (BOTTOM + PEG_VERTICAL_SPACING);
    Fix15 bottom = 480;

    for(int i =0; i < NUM_BINS; i++){
        Fix15 y = max_bin_height * fast_fix_div((Fix15)COUNTS[i], MAX_COUNT);
        Fix15 py = max_bin_height * fast_fix_div((Fix15)PREVIOUS_COUNTS[i], PREVIOUS_MAX);
        
        if (py > y) {
            fillRect(i * BIN_WIDTH, bottom - py - 2, BIN_WIDTH - 2, py - y + 2, BLACK);
        } else if (py < y) {
            fillRect(i * BIN_WIDTH, bottom - y, BIN_WIDTH - 2, y - py + 2, WHITE);
        }

        PREVIOUS_COUNTS[i] = COUNTS[i];
    }

    PREVIOUS_MAX = MAX_COUNT;
}

