// Demo of using stream.h header helpers
// to construct streams like AXI-Stream
#include "arrays.h"
#include "axi/axis.h"
#include "stream/stream.h"

// Multiple outputs as a struct
typedef struct my_func_out_t {
    //  Output .data and .valid stream
    stream(axis32_t) axis0_out;
    stream(axis32_t) axis1_out;
    //  Output ready for input axis stream
    uint1_t ready_for_axis_in;
} my_func_out_t;

my_func_out_t my_func(stream(axis32_t) input, uint1_t ready_for_axis0_out, uint1_t ready_for_axis1_out) {

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

// Demo flattened top level ports with AXIS style manager/subdorinate naming
// (could also have inputs and outputs of type stream(axis32_t)
//  but ex. Verilog does not support VHDL records...)
DECL_INPUT(uint32_t, s_axis_tdata)
DECL_INPUT(uint4_t, s_axis_tkeep)
DECL_INPUT(uint1_t, s_axis_tlast)
DECL_INPUT(uint1_t, s_axis_tvalid)
DECL_OUTPUT(uint1_t, s_axis_tready)

DECL_OUTPUT(uint32_t, m_axis0_tdata)
DECL_OUTPUT(uint4_t, m_axis0_tkeep)
DECL_OUTPUT(uint1_t, m_axis0_tlast)
DECL_OUTPUT(uint1_t, m_axis0_tvalid)
DECL_INPUT(uint1_t, m_axis0_tready)

DECL_OUTPUT(uint32_t, m_axis1_tdata)
DECL_OUTPUT(uint4_t, m_axis1_tkeep)
DECL_OUTPUT(uint1_t, m_axis1_tlast)
DECL_OUTPUT(uint1_t, m_axis1_tvalid)
DECL_INPUT(uint1_t, m_axis1_tready)

// Demo connecting two my_func's back to back
// Shows how to connect typical feed forward data
// and special FEEDBACK wires unique to hardware ready flow control
#pragma MAIN_MHZ top 100.0
void top() {
    // Connect top level input ports to local stream type variables
    //  Input stream data
    stream(axis32_t) input_axis;
    UINT_TO_BYTE_ARRAY(input_axis.data.tdata, 4, s_axis_tdata)
    UINT_TO_BIT_ARRAY(input_axis.data.tkeep, 4, s_axis_tkeep)
    input_axis.data.tlast = s_axis_tlast;
    input_axis.valid = s_axis_tvalid;

    // Input stream into first instance
    my_func_out_t func0 = my_func(input_axis, m_axis0_tready, m_axis1_tready);

    // Connect top level output ports from local stream type variables
    //  Output stream data
    m_axis0_tdata = uint8_array4_le(func0.axis0_out.data.tdata); // Array to uint
    m_axis0_tkeep = uint1_array4_le(func0.axis0_out.data.tkeep); // Array to uint
    m_axis0_tlast = func0.axis0_out.data.tlast;
    m_axis0_tvalid = func0.axis0_out.valid;

    m_axis1_tdata = uint8_array4_le(func0.axis1_out.data.tdata); // Array to uint
    m_axis1_tkeep = uint1_array4_le(func0.axis1_out.data.tkeep); // Array to uint
    m_axis1_tlast = func0.axis1_out.data.tlast;
    m_axis1_tvalid = func0.axis1_out.valid;



    //  Input stream ready
    s_axis_tready = func0.ready_for_axis_in;
}
