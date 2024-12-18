#include "uintN_t.h" // uintN_t types for any N

// 'Called'/'Executing' every 83.33ns (12MHz)
#pragma MAIN_MHZ bcd 12
#pragma MAIN bcd

uint8_t bcd(uint8_t binary_in) {

    uint8_t bin_modify = binary_in;
    uint8_t bcd = 0;
    uint4_t i = 0;
    for (i=8; i>1; i = i - 1){

        uint4_t ones = bcd & 0xF;
        uint4_t tens = (bcd & 0xF0)>>4;
        if (ones >= 5 ){
            ones += 3;
        }
        if (tens >= 5 ){
            tens += 3;
        }     
        
        bcd = (tens << 4) | ones; 
        //bcd = bcd << 4;
        //bcd = bcd | ones;

        bcd = bcd << 1;
        bcd |= bin_modify & 0x1;
        bin_modify = bin_modify >> 1;
    }
    return bcd;
}
