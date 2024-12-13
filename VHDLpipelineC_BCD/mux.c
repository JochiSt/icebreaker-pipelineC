#include "uintN_t.h"  // uintN_t types for any N


uint4_t blink(uint1_t sel, uint4_t A, uint4_t B)
{
    if (sel == 0) {
        return A;
    }else{
        return B;
    }
}
