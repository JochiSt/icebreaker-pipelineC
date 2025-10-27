// based on handshake_io.c of pipelineC examples

#include "arrays.h"
#include "handshake/handshake.h"

// AXI building blocks are a work in progress
// see ex. axi/axis.h
// But for now just define a little demo 'my' type here
typedef struct my_axis_32_t {
    uint8_t data[4];
    uint1_t keep[4];
    uint1_t last;
} my_axis_32_t;

// declare stream and handshakes
DECL_STREAM_TYPE(my_axis_32_t)
DECL_HANDSHAKE_TYPE(my_axis_32_t)
DECL_HANDSHAKE_INST_TYPE(my_axis_32_t, my_axis_32_t) // out type, in type

// Single cycle stateful(static locals) function (a state machine)
// for working with an input and output handshake
hs_out(my_axis_32_t) my_func(hs_in(my_axis_32_t) inputs) {

    hs_out(my_axis_32_t) outputs; // Default value all zeros

    // Static = register
    static stream(my_axis_32_t) buff;
    uint32_t buff_data;

    outputs.stream_out = buff;
    outputs.ready_for_stream_in = ~buff.valid;

    // Input ready writes buffer and do some calculations
    if (outputs.ready_for_stream_in) {
        buff = inputs.stream_in;
    }

    buff_data = uint8_array4_le(buff.data.data);
    buff_data += 1;
    UINT_TO_BYTE_ARRAY(buff.data.data,4,buff_data)

    // Output ready clears buffer
    if (inputs.ready_for_stream_out) {
        buff.valid = 0;
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
    // func0: my_axis_32_t my_func(my_axis_32_t)
    DECL_HANDSHAKE_INST(func0, my_axis_32_t, my_func, my_axis_32_t)
    // func1: my_axis_32_t my_func(my_axis_32_t)
    DECL_HANDSHAKE_INST(func1, my_axis_32_t, my_func, my_axis_32_t)

    // Connect flattened top level input ports to local stream variables
    stream(my_axis_32_t) input_axis;
    UINT_TO_BYTE_ARRAY(input_axis.data.data, 4, s_axis_tdata)
    UINT_TO_BIT_ARRAY(input_axis.data.keep, 4, s_axis_tkeep)
    input_axis.data.last = s_axis_tlast;
    input_axis.valid = s_axis_tvalid;

    // Input stream into first instance
    // func0 input handshake = input_axis, s_axis_tready
    HANDSHAKE_FROM_STREAM(func0, input_axis, s_axis_tready)

    // Output of first instance into second
    // func1 input handshake = func0 output handshake
    HANDSHAKE_CONNECT(func1, func0)

    // Output stream from second instance
    stream(my_axis_32_t) output_axis;
    // output_axis, m_axis_tready = func1 output handshake
    STREAM_FROM_HANDSHAKE(output_axis, m_axis_tready, func1)

    // Connect flattened top level output ports from local stream type variables
    m_axis_tdata = uint8_array4_le(output_axis.data.data); // Array to uint
    m_axis_tkeep = uint1_array4_le(output_axis.data.keep); // Array to uint
    m_axis_tlast = output_axis.data.last;
    m_axis_tvalid = output_axis.valid;
}
