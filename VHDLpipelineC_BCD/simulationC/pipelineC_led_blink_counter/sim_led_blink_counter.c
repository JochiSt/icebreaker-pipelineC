#include <stdio.h>

#include "led_blink_counter.h"

int main(void){
    printf("Hello LED blink counter test\n");

    uint1_t led = 0;
    uint1_t old_led = 0;

    for(long clock_cycle = 0; clock_cycle < 50000000; clock_cycle++) {

        led = led_blink_counter();
        if(led != old_led){
            printf("LED changed at %ld to %d\n", clock_cycle, led);
        }
        old_led = led;

    }

    return 0;
}

