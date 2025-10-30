// Demo of using stream.h header helpers
// to construct streams like AXI-Stream
#include "arrays.h"
#include "axi/axis.h"
#include "stream/stream.h"

// Multiple outputs as a struct
typedef struct my_func_out_t {
    //  Output .data and .valid stream
    stream(axis32_t) axis_out;
    //  Output ready for input axis stream
    uint1_t ready_for_axis_in;
} my_func_out_t;

my_func_out_t my_func(stream(axis32_t) inputs, uint1_t ready_for_axis_out) {

    static my_func_out_t outputs; // Default value all zeros

    static uint1_t new_data = 0;

    // if we have no new data -> ready for new data
    outputs.ready_for_axis_in = ~new_data;
    outputs.axis_out.data.tlast = 0;

    // Input ready writes buffer and do some calculations
    if (inputs.valid) {
        if (~new_data) {
            uint32_t buff_data;
            buff_data = uint8_array4_le(inputs.data.tdata);
            buff_data += 1;
            UINT_TO_BYTE_ARRAY(outputs.axis_out.data.tdata, 4, buff_data)

            new_data = 1; // mark, that new data is available
        }
    }

    // Output ready marks, that we are ready for new data
    if (new_data) {
        if (ready_for_axis_out) {
            new_data = 0; // data has been read out
        }
        outputs.axis_out.valid = 1;
    } else {
        outputs.axis_out.valid = 0;
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

DECL_OUTPUT(uint32_t, m_axis_tdata)
DECL_OUTPUT(uint4_t, m_axis_tkeep)
DECL_OUTPUT(uint1_t, m_axis_tlast)
DECL_OUTPUT(uint1_t, m_axis_tvalid)
DECL_INPUT(uint1_t, m_axis_tready)

// Demo connecting two my_func's back to back
// Shows how to connect typical feed forward data
// and special FEEDBACK wires unique to hardware ready flow control
#pragma PART "xc7a35ticsg324-1l" // Artix 7 35T (Arty)
#pragma MAIN_MHZ top 100.0
void top() {
    // Connect top level input ports to local stream type variables
    //  Input stream data
    stream(axis32_t) input_axis;
    UINT_TO_BYTE_ARRAY(input_axis.data.tdata, 4, s_axis_tdata)
    UINT_TO_BIT_ARRAY(input_axis.data.tkeep, 4, s_axis_tkeep)
    input_axis.data.tlast = s_axis_tlast;
    input_axis.valid = s_axis_tvalid;
    //  Output stream ready
    uint1_t ready_for_output_axis = m_axis_tready;

    // Input stream into first instance
    uint1_t ready_for_func0_axis_out;
// Note: FEEDBACK not assigned a value yet
#pragma FEEDBACK ready_for_func0_axis_out
    my_func_out_t func0 = my_func(
        input_axis,
        ready_for_func0_axis_out);
    uint1_t ready_for_input_axis = func0.ready_for_axis_in;

    // Output of first instance into second
    uint1_t ready_for_func1_axis_out = ready_for_output_axis;
    my_func_out_t func1 = my_func(
        func0.axis_out,
        ready_for_func1_axis_out);
    // Note: FEEDBACK assigned here
    ready_for_func0_axis_out = func1.ready_for_axis_in;

    // Connect top level output ports from local stream type variables
    //  Output stream data
    m_axis_tdata = uint8_array4_le(func1.axis_out.data.tdata); // Array to uint
    m_axis_tkeep = uint1_array4_le(func1.axis_out.data.tkeep); // Array to uint
    m_axis_tlast = func1.axis_out.data.tlast;
    m_axis_tvalid = func1.axis_out.valid;
    //  Input stream ready
    s_axis_tready = ready_for_input_axis;
}

// TODO testbench with printfs?