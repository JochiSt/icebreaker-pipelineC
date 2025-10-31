#pragma once

#include "intN_t.h"
#include "arrays.h"
#include "axi/axis.h"
#include "stream/stream.h"

// Multiple outputs as a struct
typedef struct my_func_out_t {
    //  Output .data and .valid stream
    stream(eth8_frame_t) axis0_out;
    stream(eth8_frame_t) axis1_out;
    //  Output ready for input axis stream
    uint1_t ready_for_axis_in;
} my_func_out_t;

my_func_out_t ip_parser(stream(eth8_frame_t) input, uint1_t ready_for_axis0_out, uint1_t ready_for_axis1_out) {

    static my_func_out_t outputs; // Default value all zeros
    static uint1_t new_data0 = 0;
    static uint1_t new_data1 = 0;

    // if we have no new data -> ready for new data
    outputs.ready_for_axis_in = ~ ( new_data0 | new_data1);

    // set TLAST to zero
    outputs.axis0_out.data.tlast = 0;
    outputs.axis1_out.data.tlast = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Input ready writes buffer and do some calculations
    if (input.valid) {
        uint32_t buff_data;
        buff_data = uint8_array4_le(input.data.tdata);
        if (~new_data0 & ( (buff_data & 0x1) == 0) ) {      // buffer 0 should have all even ones
            UINT_TO_BYTE_ARRAY(outputs.axis0_out.data.tdata, 4, buff_data)
            new_data0 = 1; // mark, that new data is available
        }else if (~new_data1) {                             // buffer 1 should have all odd ones
            UINT_TO_BYTE_ARRAY(outputs.axis1_out.data.tdata, 4, buff_data)
            new_data1 = 1; // mark, that new data is available
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Output ready marks, that we are ready for new data
    if (new_data0) {
        if (ready_for_axis0_out) {
            new_data0 = 0; // data has been read out
        }
        outputs.axis0_out.valid = 1;
    } else {
        outputs.axis0_out.valid = 0;
    }

    ////////////////////////////////////////////////////////////////////////////
    if (new_data1) {
        if (ready_for_axis1_out) {
            new_data1 = 0; // data has been read out
        }
        outputs.axis1_out.valid = 1;
    } else {
        outputs.axis1_out.valid = 0;
    }

    return outputs;
}