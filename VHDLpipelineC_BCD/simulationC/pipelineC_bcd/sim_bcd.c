#include <stdio.h>

#include "bcd.h"

int main(void) {
    printf("Hello BCD test\n");

    for (uint8_t binary_in = 0; binary_in < 100; binary_in++) {
        printf("input number: %02d \t -> \t %02X\n", binary_in, bcd(binary_in));
    }

    return 0;
}
