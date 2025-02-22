#include "dma.h"

#include <math.h>

void init_DMA() {
    int i ;
      for (i=0; i<(sine_table_size); i++){
          raw_sin[i] = (int)(2047 * sin((float)i*6.283/(float)sine_table_size) + 2047); //12 bit
          DAC_data[i] = DAC_config_chan_A | (raw_sin[i] & 0x0fff) ;
      }
    // Select DMA channels
    data_chan = dma_claim_unused_channel(true);;
    ctrl_chan = dma_claim_unused_channel(true);;
  
    // Setup the control channel
    dma_channel_config c = dma_channel_get_default_config(ctrl_chan);   // default configs
    channel_config_set_transfer_data_size(&c, DMA_SIZE_32);             // 32-bit txfers
    channel_config_set_read_increment(&c, false);                       // no read incrementing
    channel_config_set_write_increment(&c, false);                      // no write incrementing
    channel_config_set_chain_to(&c, data_chan);                         // chain to data channel
  
    dma_channel_configure(
        ctrl_chan,                          // Channel to be configured
        &c,                                 // The configuration we just created
        &dma_hw->ch[data_chan].read_addr,   // Write address (data channel read address)
        &address_pointer,                   // Read address (POINTER TO AN ADDRESS)
        1,                                  // Number of transfers
        false                               // Don't start immediately
    );
  
    // Setup the data channel
    dma_channel_config c2 = dma_channel_get_default_config(data_chan);  // Default configs
    channel_config_set_transfer_data_size(&c2, DMA_SIZE_16);            // 16-bit txfers
    channel_config_set_read_increment(&c2, true);                       // yes read incrementing
    channel_config_set_write_increment(&c2, false);                     // no write incrementing
    // (X/Y)*sys_clk, where X is the first 16 bytes and Y is the second
    // sys_clk is 125 MHz unless changed in code. Configured to ~44 kHz
    dma_timer_set_fraction(0, 0x0017, 0xffff) ;
    // 0x3b means timer0 (see SDK manual)
    channel_config_set_dreq(&c2, 0x3b);                                 // DREQ paced by timer 0
    // chain to the controller DMA channel
  
    //channel_config_set_chain_to(&c2, ctrl_chan);                       CHANGED
  
  
    dma_channel_configure(
        data_chan,                  // Channel to be configured
        &c2,                        // The configuration we just created
        &spi_get_hw(SPI_PORT)->dr,  // write address (SPI data register)
        DAC_data,                   // The initial read address
        sine_table_size,            // Number of transfers
        false                       // Don't start immediately.
    );
  
  
    // start the control channel
    //dma_start_channel_mask(1u << ctrl_chan) ;
  
  };
  void init_SPI() {
     // Initialize SPI channel (channel, baud rate set to 20MHz)
     spi_init(SPI_PORT, 20000000) ;
  
     // Format SPI channel (channel, data bits per transfer, polarity, phase, order)
     spi_set_format(SPI_PORT, 16, 0, 0, 0);
  
     // Map SPI signals to GPIO ports, acts like framed SPI with this CS mapping
     gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
     gpio_set_function(PIN_CS, GPIO_FUNC_SPI) ;
     gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
     gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
  
  };
  
  /**GPT **/
  void start_audio() {
    dma_channel_set_read_addr(data_chan, DAC_data, true); // Reset read pointer
    dma_channel_set_trans_count(data_chan, sine_table_size, true); // Reset transfer count
    dma_channel_start(data_chan);  // Start the data channel manually

    dma_channel_start(ctrl_chan);
  }
  