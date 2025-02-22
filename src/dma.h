#ifndef DMA_H
#define DMA_H

#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"


// == sin table ========================================
// Number of samples per period in sine table
#define sine_table_size 256

// Sine table
static int raw_sin[sine_table_size];

// Table of values to be sent to DAC
static unsigned short DAC_data[sine_table_size];

// Pointer to the address of the DAC data table
static unsigned short *address_pointer = &DAC_data[0];

// A-channel, 1x, active
#define DAC_config_chan_A 0b0011000000000000

// SPI configurations
#define PIN_MISO 4
#define PIN_CS 5
#define PIN_SCK 6
#define PIN_MOSI 7
#define SPI_PORT spi0

// Number of DMA transfers per event
static const uint32_t transfer_count = sine_table_size;

static int data_chan, ctrl_chan;    // Select DMA channels

void init_DMA();
void init_SPI();

/**GPT **/
void start_audio();

#endif // DMA_H