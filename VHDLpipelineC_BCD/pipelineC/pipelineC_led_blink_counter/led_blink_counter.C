#include "uintN_t.h" // uintN_t types for any N

// Count to 12000000 iterations * 83.2ns each ~= 1sec
uint25_t counter;

// LED on off state
uint1_t led = 0;

// 'Called'/'Executing' every 83.2ns  (12MHz)
#pragma MAIN_MHZ led_blink_counter 12

uint1_t led_blink_counter() {
    // If reached 1 second
    if (counter == (12000000 - 1)) {
        led = !led;  // Toggle led
        counter = 0; // Reset counter
    } else {
        counter += 1; // one 30ns increment
    }
    return led;
}
