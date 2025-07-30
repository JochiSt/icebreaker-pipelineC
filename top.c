// See configuration details like top level pin mapping in top.h
//#define DEFAULT_PI_UART
//#define DEFAULT_VGA_PMOD
#include "top.h"

#include "pinout/pinout_LEDs.h"

// Blinky part of demo
#define HALF_SEC ((uint32_t)((PLL_CLK_MHZ*1000000.0)/2.0))
#define count_t uint32_t
MAIN_MHZ(blinky_main, PLL_CLK_MHZ)
void blinky_main(){
  static count_t counter;
  static uint1_t led_reg;
  LED1 = led_reg;

  counter += 1;
  if(counter >= HALF_SEC){
    counter = 0;
    led_reg = ~led_reg;
  }
}

// UART part of demo
MAIN_MHZ(uart_main, PLL_CLK_MHZ)
void uart_main(){
  // Default loopback connect
  uart_tx_mac_word_in = uart_rx_mac_word_out;
  uart_rx_mac_out_ready = uart_tx_mac_in_ready;

  // Override .data to do case change demo
  char in_char = uart_rx_mac_word_out.data;
  char out_char = in_char;
  uint8_t case_diff = 'a' - 'A';
  if(in_char >= 'a' && in_char <= 'z'){
    out_char = in_char - case_diff;
  }else if(in_char >= 'A' && in_char <= 'Z'){
    out_char = in_char + case_diff;
  }
  uart_tx_mac_word_in.data = out_char;
}


