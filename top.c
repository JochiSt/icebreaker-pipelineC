// See configuration details like top level pin mapping in top.h
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
