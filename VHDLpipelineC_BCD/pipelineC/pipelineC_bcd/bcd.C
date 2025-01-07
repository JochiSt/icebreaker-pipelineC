#include "uintN_t.h" // uintN_t types for any N

// 'Called'/'Executing' every 83.33ns (12MHz)
#pragma MAIN_MHZ bcd 12
#pragma MAIN bcd

uint8_t bcd(uint8_t binary_in) {

    uint4_t tens = binary_in / 10;
    uint4_t ones = binary_in % 10;

    uint8_t bcd = tens << 4;
    bcd = bcd | ones;
    
    return bcd;
}
