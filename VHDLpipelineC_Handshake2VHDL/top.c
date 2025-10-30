// based on handshake_io.c of pipelineC examples

#include "arrays.h"
#include "axi/axis.h"
#include "handshake/handshake.h"

// declare handshakes
// DECL_STREAM_TYPE(axis32_t)
DECL_HANDSHAKE_TYPE(axis32_t)
DECL_HANDSHAKE_INST_TYPE(axis32_t, axis32_t) // out type, in type

// Single cycle stateful(static locals) function (a state machine)
// for working with an input and output handshake
hs_out(axis32_t) my_func(hs_in(axis32_t) inputs) {

    static hs_out(axis32_t) outputs; // Default value all zeros

    static uint1_t new_data = 0;

    // if we have no new data -> ready for new data
    outputs.ready_for_stream_in = ~new_data;

    // Input ready writes buffer and do some calculations
    if (inputs.stream_in.valid) {
        if (~new_data) {
            uint32_t buff_data;
            buff_data = uint8_array4_le(inputs.stream_in.data.tdata);
            buff_data += 1;
            UINT_TO_BYTE_ARRAY(outputs.stream_out.data.tdata, 4, buff_data)

            new_data = 1; // mark, that new data is available
        }
    }

    // Output ready marks, that we are ready for new data
    if (new_data) {
        if (inputs.ready_for_stream_out) {
            new_data = 0; // data has been read out
        }
        outputs.stream_out.valid = 1;
    } else {
        outputs.stream_out.valid = 0;
    }

    return outputs;
}

////////////////////////////////////////////////////////////////////////////////
// SLAVE AXI = input
DECL_INPUT(uint32_t, s_axis_tdata)
DECL_INPUT(uint4_t, s_axis_tkeep)
DECL_INPUT(uint1_t, s_axis_tlast)
DECL_INPUT(uint1_t, s_axis_tvalid)
DECL_OUTPUT(uint1_t, s_axis_tready)

// MASTER AXI = output
DECL_OUTPUT(uint32_t, m_axis_tdata)
DECL_OUTPUT(uint4_t, m_axis_tkeep)
DECL_OUTPUT(uint1_t, m_axis_tlast)
DECL_OUTPUT(uint1_t, m_axis_tvalid)
DECL_INPUT(uint1_t, m_axis_tready)

// Demo connecting two my_func's back to back
#pragma MAIN_MHZ top 100.0
void top() {
    // Handshake style code with helper macros etc
    // Requires instantiating the modules to be used first at start of func
    // func0: axis32_t my_func(axis32_t)
    DECL_HANDSHAKE_INST(func0, axis32_t, my_func, axis32_t)
    // func1: axis32_t my_func(axis32_t)
    DECL_HANDSHAKE_INST(func1, axis32_t, my_func, axis32_t)

    // Connect flattened top level input ports to local stream variables
    stream(axis32_t) input_axis;
    UINT_TO_BYTE_ARRAY(input_axis.data.tdata, 4, s_axis_tdata)
    UINT_TO_BIT_ARRAY(input_axis.data.tkeep, 4, s_axis_tkeep)
    input_axis.data.tlast = s_axis_tlast;
    input_axis.valid = s_axis_tvalid;

    // Input stream into first instance
    // func0 input handshake = input_axis, s_axis_tready
    HANDSHAKE_FROM_STREAM(func0, input_axis, s_axis_tready)

    // Output of first instance into second
    // func1 input handshake = func0 output handshake
    HANDSHAKE_CONNECT(func1, func0)

    // Output stream from second instance
    stream(axis32_t) output_axis;
    // output_axis, m_axis_tready = func1 output handshake
    STREAM_FROM_HANDSHAKE(output_axis, m_axis_tready, func1)

    // Connect flattened top level output ports from local stream type variables
    m_axis_tdata = uint8_array4_le(output_axis.data.tdata); // Array to uint
    m_axis_tkeep = uint1_array4_le(output_axis.data.tkeep); // Array to uint
    m_axis_tlast = output_axis.data.tlast;
    m_axis_tvalid = output_axis.valid;
}
