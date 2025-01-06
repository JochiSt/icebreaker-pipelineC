#include "uintN_t.h"  // uintN_t types for any N

#pragma MAIN_MHZ mux 100

uint4_t mux(uint1_t sel, uint4_t A, uint4_t B)
{
    uint4_t retval = 0;
    if (sel == 0) {
        retval = A;
    }else{
        retval = B;
    }
    return retval;
}
