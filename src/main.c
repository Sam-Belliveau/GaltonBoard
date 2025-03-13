
/**
 * Hunter Adams (vha3@cornell.edu)
 *
 * This demonstration animates two balls bouncing about the screen.
 * Through a serial interface, the user can change the ball color.
 *
 * HARDWARE CONNECTIONS
 *  - GPIO 16 ---> VGA Hsync
 *  - GPIO 17 ---> VGA Vsync
 *  - GPIO 18 ---> 470 ohm resistor ---> VGA Green
 *  - GPIO 19 ---> 330 ohm resistor ---> VGA Green
 *  - GPIO 20 ---> 330 ohm resistor ---> VGA Blue
 *  - GPIO 21 ---> 330 ohm resistor ---> VGA Red
 *  - RP2040 GND ---> VGA GND
 *
 * RESOURCES USED
 *  - PIO state machines 0, 1, and 2 on PIO instance 0
 *  - DMA channels (2, by claim mechanism)
 *  - 153.6 kBytes of RAM (for pixel color data)
 *
 */

// Include the VGA grahics library
#include "vga16_graphics.h"
// Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Include Pico libraries
#include "pico/stdlib.h"
#include "pico/divider.h"
#include "pico/multicore.h"
#include "hardware/sync.h"

// Include hardware libraries
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/adc.h"

// Include protothreads
#include "pt_cornell_rp2040_v1_3.h"

// === the fixed point macros ========================================
#include "fix15.h"
#include "ball.h"
#include "peg.h"
#include "dma.h"
#include "draw_stats.h"
#include "tma.h"

#define OVERCLOCK_MUL 2

#define FRAME_RATE 33000

static tma_t ball_count_tma;
static tma_t physics_us_tma;
static tma_t drawing_us_tma;

// Animation on core 0
static PT_THREAD(protothread_anim(struct pt *pt))
{
  // Mark beginning of thread
  PT_BEGIN(pt);

  // Variables for maintaining frame rate
  static int frame_begin_time;
  static int spare_time;

  static int start_time;

  static int state;

  init_balls(); 
  init_pegs();

  tma_init(&ball_count_tma);
  tma_init(&physics_us_tma);

  start_time = time_us_32();

  static int pread = 0;
  while (1)
  {
    // Measure time at start of thread
    frame_begin_time = time_us_32();

    tma_add(&ball_count_tma, adc_read());

    const int new_ball_count = tma_get(&ball_count_tma); //Get avged ball count

    int next = gpio_get(15);

    if (pread != next)
    {
      if (pread == 0)
      {

        state = !!!state;
      }
      pread = next;
    }

    if (state == 0)
    {
      int new_ball_real = new_ball_count * MAX_BALL_COUNT / 4096;
      if (abs(new_ball_real - get_ball_count()) > 10)
      {
        set_ball_count(new_ball_real);

        for (int i = 0; i < NUM_BINS; i++)
        {
          COUNTS[i] = 0;
        }

        MAX_COUNT = 1;
        TOTAL_COUNT = 0;
      }
    }
    else
    {
      int prev_bounce = BOUNCE * 4096;

      if (abs(new_ball_count - prev_bounce) > 10)
      {
        BOUNCE = ((Fix15)new_ball_count) / 4096;

        for (int i = 0; i < NUM_BINS; i++)
        {
          COUNTS[i] = 0;
        }

        MAX_COUNT = 1;
        TOTAL_COUNT = 0;
      }
    }


    int ptime = tma_get(&physics_us_tma);
    int dtime = tma_get(&drawing_us_tma);

    draw_stats(frame_begin_time - start_time, ptime, dtime, state);
    
    gpio_put(25, ptime > FRAME_RATE|| dtime > FRAME_RATE);

    draw_histogram();
    draw_pegs();
    clear_draw_balls();
    __sync_synchronize();

    const int drawing_us = time_us_32() - frame_begin_time;
    tma_add(&drawing_us_tma, drawing_us);
    spare_time = FRAME_RATE - drawing_us;
    // yield for necessary amount odraw_usf time
    PT_YIELD_usec(spare_time);
    // NEVER exit while
  } // END WHILE(1)
  PT_END(pt);
} // animation thread

// Physics on core 1
static PT_THREAD(protothread_physics(struct pt *pt))
{
  // Mark beginning of thread
  PT_BEGIN(pt);

  // Variables for maintaining frame rate
  static int begin_time;
  static int spare_time;

  while (1)
  {
    // Measure time at start of thread
    begin_time = time_us_32();

    int ptime = tma_get(&physics_us_tma);
    int dtime = tma_get(&drawing_us_tma);


    update_balls();
    __sync_synchronize();

    const int physics_us = time_us_32() - begin_time;
    tma_add(&physics_us_tma, physics_us);

    // yield for necessary amount of time
    spare_time = FRAME_RATE - (time_us_32() - begin_time);
    PT_YIELD_usec(spare_time);
    // NEVER exit while
  } // END WHILE(1)
  PT_END(pt);
} // animation thread

// ========================================
// === core 1 main -- started in main below
// ========================================
void core1_main()
{
  // Add animation thread
  // Start the scheduler

  pt_schedule_start;
}

// ========================================
// === main
// ========================================
// USE ONLY C-sdk library
int main()
{
  set_sys_clock_khz(OVERCLOCK_MUL * 125000, true);
  // initialize stio
  stdio_init_all();

  reset_counts();
  init_balls();

  adc_init();
  // Initialize GPIO 26 (ADC0) for analog input
  adc_gpio_init(26);
  // Select ADC input 0 (corresponds to GP26)
  adc_select_input(0);

  gpio_init(15);
  gpio_set_dir(15, GPIO_IN);

  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);

  // initialize VGA
  initVGA();
  // initialize SPI
  init_SPI();
  // initialize DMA
  init_DMA();

  // start core 1
  multicore_reset_core1();
  multicore_launch_core1(&core1_main);

  pt_add_thread(protothread_physics);
  pt_add_thread(protothread_anim);

  // add threads
  // start scheduler
  pt_schedule_start;
}
