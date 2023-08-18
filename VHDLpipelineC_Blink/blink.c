#include "uintN_t.h"  // uintN_t types for any N

// Count to 33333333 iterations * 30ns each ~= 1sec
uint25_t counter;

// LED on off state
uint1_t led5;

// 'Called'/'Executing' every 30ns (33.33MHz)
#pragma MAIN_MHZ blink 12
#pragma MAIN blink

uint1_t blink()
{
  // If reached 1 second
  if(counter==(12000000-1))
  {
    // Toggle led
    led5 = !led5;
    // Reset counter
    counter = 0;
  }
  else
  {
    counter += 1; // one 30ns increment
  }
  return led5;
}
