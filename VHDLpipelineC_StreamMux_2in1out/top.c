// Demo of using stream.h header helpers
// to construct streams like AXI-Stream
#include "arrays.h"
#include "axi/axis.h"
#include "stream/stream.h"

// Multiple outputs as a struct
typedef struct my_func_out_t {
    //  Output .data and .valid stream
    stream(axis32_t) axis0_out;
    //  Output ready for input axis stream
    uint1_t ready_for_axis0_in;
    uint1_t ready_for_axis1_in;
} my_func_out_t;

my_func_out_t my_func(stream(axis32_t) input0, stream(axis32_t) input1, uint1_t ready_for_axis0_out) {

    static my_func_out_t outputs; // Default value all zeros
    static uint1_t new_data0 = 0;
    static uint1_t new_data1 = 0;

    static stream(axis32_t) data0;
    static stream(axis32_t) data1;

    // if we have no new data -> ready for new data
    outputs.ready_for_axis0_in = ~new_data0;
    outputs.ready_for_axis1_in = ~new_data1;

    // set TLAST to zero
    outputs.axis0_out.data.tlast = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Input ready writes buffer and do some calculations
    if (input0.valid) {
        if (~new_data0) {
            data0 = input0;
            new_data0 = 1; // mark, that new data is available
        }
        if (~new_data1) {
            data1 = input0;
            new_data1 = 1; // mark, that new data is available
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Output ready marks, that we are ready for new data
    uint1_t data_valid = 0;
    if (new_data0) {
        if (ready_for_axis0_out) {
            outputs.axis0_out = data0;
            data_valid = 1;
            new_data0 = 0; // data has been read out
        }
    }
    if (new_data1 & ~ data_valid) {
        if (ready_for_axis0_out) {
            outputs.axis0_out = data1;
            data_valid = 1;
            new_data1 = 0; // data has been read out
        }
    }
    outputs.axis0_out.valid = data_valid;

    ////////////////////////////////////////////////////////////////////////////

    return outputs;
}

// Demo flattened top level ports with AXIS style manager/subdorinate naming
// (could also have inputs and outputs of type stream(axis32_t)
//  but ex. Verilog does not support VHDL records...)
DECL_INPUT(uint32_t, s_axis0_tdata)
DECL_INPUT(uint4_t, s_axis0_tkeep)
DECL_INPUT(uint1_t, s_axis0_tlast)
DECL_INPUT(uint1_t, s_axis0_tvalid)
DECL_OUTPUT(uint1_t, s_axis0_tready)

DECL_INPUT(uint32_t, s_axis1_tdata)
DECL_INPUT(uint4_t, s_axis1_tkeep)
DECL_INPUT(uint1_t, s_axis1_tlast)
DECL_INPUT(uint1_t, s_axis1_tvalid)
DECL_OUTPUT(uint1_t, s_axis1_tready)

DECL_OUTPUT(uint32_t, m_axis0_tdata)
DECL_OUTPUT(uint4_t, m_axis0_tkeep)
DECL_OUTPUT(uint1_t, m_axis0_tlast)
DECL_OUTPUT(uint1_t, m_axis0_tvalid)
DECL_INPUT(uint1_t, m_axis0_tready)

// Demo connecting two my_func's back to back
// Shows how to connect typical feed forward data
// and special FEEDBACK wires unique to hardware ready flow control
#pragma MAIN_MHZ top 100.0
void top() {
    // Connect top level input ports to local stream type variables
    //  Input stream data
    stream(axis32_t) input_axis0;
    UINT_TO_BYTE_ARRAY(input_axis0.data.tdata, 4, s_axis0_tdata)
    UINT_TO_BIT_ARRAY(input_axis0.data.tkeep, 4, s_axis0_tkeep)
    input_axis0.data.tlast = s_axis0_tlast;
    input_axis0.valid = s_axis0_tvalid;

    stream(axis32_t) input_axis1;
    UINT_TO_BYTE_ARRAY(input_axis1.data.tdata, 4, s_axis1_tdata)
    UINT_TO_BIT_ARRAY(input_axis1.data.tkeep, 4, s_axis1_tkeep)
    input_axis1.data.tlast = s_axis1_tlast;
    input_axis1.valid = s_axis1_tvalid;

    // Input stream into first instance
    my_func_out_t func0 = my_func(input_axis0, input_axis1, m_axis0_tready);

    // Connect top level output ports from local stream type variables
    //  Output stream data
    m_axis0_tdata = uint8_array4_le(func0.axis0_out.data.tdata); // Array to uint
    m_axis0_tkeep = uint1_array4_le(func0.axis0_out.data.tkeep); // Array to uint
    m_axis0_tlast = func0.axis0_out.data.tlast;
    m_axis0_tvalid = func0.axis0_out.valid;

    //  Input stream ready
    s_axis0_tready = func0.ready_for_axis0_in;
    s_axis1_tready = func0.ready_for_axis1_in;
}
