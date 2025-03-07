#include "draw_stats.h"

#include <stdio.h>

#include "vga16_graphics.h"
#include "ball.h"

int TOTAL_COUNT = 0;
int time_since_start = 0;

int MAX_COUNT = 0;
int COUNTS[NUM_BINS];

int PREVIOUS_MAX = 1;
int PREVIOUS_COUNTS[NUM_BINS];

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

void draw_stats(int32_t time_since_boot_us, int32_t physics_us, int32_t draw_us, int state)
{

    setCursor(0, 0);
    setTextSize(1);
    setTextColor2(GREEN, BLACK);
 
    char ball_count_buffer[4096];
    sprintf(ball_count_buffer, 
        "Time: %d s    \n"
        "Physics: %d us    \n"
        "Drawing: %d us    \n"
        "Total Balls: %d     \n"
        "Current Balls: %d    \n"
        "Bounciness: %d / 4096 \n"
        "Current State: %s",
        time_since_boot_us / 1000000, 
        physics_us,
        draw_us,
        TOTAL_COUNT, 
        get_ball_count(),
        (int)(BOUNCE * 4096),
        state == 0 ? "Setting Ball Count" : "Setting Bounciness"
    );
    writeString(ball_count_buffer);
}

void draw_histogram() {
    
    Fix15 bottom = 450;
    Fix15 max_bin_height = bottom - (BOTTOM + PEG_VERTICAL_SPACING) - 20;

    for(int i =0; i < NUM_BINS; i++){
        Fix15 y = max_bin_height * fast_fix_div((Fix15)COUNTS[i], MAX_COUNT);
        Fix15 py = max_bin_height * fast_fix_div((Fix15)PREVIOUS_COUNTS[i], PREVIOUS_MAX);
        
        char ball_cat_count[256];
        if (py > y) {
            fillRect(i * BIN_WIDTH, bottom - py - 1, BIN_WIDTH - 1, py - y + 1, BLACK);
            fillRect(i * BIN_WIDTH, bottom - y, BIN_WIDTH - 1, y - py + 1, WHITE);
        } 
        else if (py < y) {
            fillRect(i * BIN_WIDTH, bottom - y, BIN_WIDTH - 1, y - py + 1, WHITE);
        }

        if (PREVIOUS_COUNTS[i] != COUNTS[i]) {
            setCursor(i * BIN_WIDTH+10, bottom + 20);
            setTextSize(1);
            setTextColor2(WHITE, BLACK);
            sprintf(ball_cat_count, "%d  ", COUNTS[i]);
            writeString(ball_cat_count);
        }

        PREVIOUS_COUNTS[i] = COUNTS[i];
    }

    PREVIOUS_MAX = MAX_COUNT;
}

