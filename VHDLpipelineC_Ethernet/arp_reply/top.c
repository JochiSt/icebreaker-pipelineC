// See configuration details like top level pin mapping in top.h
#include "top.h"

////////////////////////////////////////////////////////////////////////////////
//      ____  _      _____ _  ___   ___     __
//     |  _ \| |    |_   _| |/ / \ | \ \   / /
//     | |_) | |      | | | ' /|  \| |\ \_/ /
//     |  _ <| |      | | |  < | . ` | \   /
//     | |_) | |____ _| |_| . \| |\  |  | |
//     |____/|______|_____|_|\_\_| \_|  |_|
//
////////////////////////////////////////////////////////////////////////////////

#include "../pinout/pinout_LEDs.h"
#define HALF_SEC ((uint32_t)((PLL_CLK_MHZ * 1000000.0) / 2.0))
#define count_t uint32_t
MAIN_MHZ(blinky_main, PLL_CLK_MHZ)
void blinky_main() {
    static count_t counter;
    static uint1_t led_reg;
    LED1 = led_reg;

    counter += 1;
    if (counter >= HALF_SEC) {
        counter = 0;
        led_reg = ~led_reg;
    }
}

////////////////////////////////////////////////////////////////////////////////
//      _    _         _____ _______
//     | |  | |  /\   |  __ \__   __|
//     | |  | | /  \  | |__) | | |
//     | |  | |/ /\ \ |  _  /  | |
//     | |__| / ____ \| | \ \  | |
//      \____/_/    \_\_|  \_\ |_|
//
////////////////////////////////////////////////////////////////////////////////

#include "../pinout/pinout_UART.h"
#define UART_CLK_MHZ PLL_CLK_MHZ
#define UART_BAUD 115200
#include "uart/uart_mac.c"

// UART part of demo
MAIN_MHZ(uart_main, PLL_CLK_MHZ)
void uart_main() {
    // Default loopback connect
    uart_tx_mac_word_in = uart_rx_mac_word_out;
    uart_rx_mac_out_ready = uart_tx_mac_in_ready;

    // Override .data to do case change demo
    char in_char = uart_rx_mac_word_out.data;
    char out_char = in_char;
    uint8_t case_diff = 'a' - 'A';
    if (in_char >= 'a' && in_char <= 'z') {
        out_char = in_char - case_diff;
    } else if (in_char >= 'A' && in_char <= 'Z') {
        out_char = in_char + case_diff;
    }
    uart_tx_mac_word_in.data = out_char;
}

////////////////////////////////////////////////////////////////////////////////
//      ______ _______ _    _ ______ _____  _   _ ______ _______
//     |  ____|__   __| |  | |  ____|  __ \| \ | |  ____|__   __|
//     | |__     | |  | |__| | |__  | |__) |  \| | |__     | |
//     |  __|    | |  |  __  |  __| |  _  /| . ` |  __|    | |
//     | |____   | |  | |  | | |____| | \ \| |\  | |____   | |
//     |______|  |_|  |_|  |_|______|_|  \_\_| \_|______|  |_|
//
////////////////////////////////////////////////////////////////////////////////
#include "../pinout/pinout_LAN8720.h"
#include "net/rmii_wires.c"

// Include ethernet media access controller configured to use RMII wires and 8b AXIS
// with enabled clock crossing fifos (with skid buffers)
#define RMII_ETH_MAC_RX_SKID_IN_FIFO_DEPTH 4
#define RMII_ETH_MAC_TX_SKID_OUT_FIFO_DEPTH 4
#include "net/rmii_eth_mac.c"

// Include logic for parsing/building ethernet frames (8b AXIS)
#include "net/eth_8.h"
// MAC address info we want the fpga to have (shared with software)
#include "../fpga_mac.h"

// Include definition of work to compute
#include "arp.h"

// Global stream pipeline interface looks same as FIFOs
#include "global_func_inst.h"
GLOBAL_VALID_READY_PIPELINE_INST(arp_pipeline, arp_reply_t, arp, arp_request_t, 4)

// De/serialize 1 byte at a time to/from the types for the work compute
#include "stream/deserializer.h"
#include "stream/serializer.h"
// axis_packet deserialize variant to handle min eth frame size padding on incoming frames
axis_packet_to_type(arp_deserialize, 8, arp_request_t)
type_byte_serializer(arp_serialize, arp_reply_t, 1)

// ARP and regular loopback use headers FIFO
GLOBAL_STREAM_FIFO(eth_header_t, loopback_headers_fifo, 2) // another one to hold the headers

////////////////////////////////////////////////////////////////////////////////
//      _____  __   __
//     |  __ \ \ \ / /
//     | |__) | \ V /
//     |  _  /   > <
//     | | \ \  / . \
//     |_|  \_\/_/ \_\
//
////////////////////////////////////////////////////////////////////////////////
MAIN_MHZ(rx_main, PLL_CLK_MHZ) void rx_main() {
    // Eth rx ready if deser+header fifo ready
    uint1_t deser_ready_for_input;
    #pragma FEEDBACK deser_ready_for_input
    uint1_t eth_rx_out_ready = deser_ready_for_input & loopback_headers_fifo_in_ready;

    // The rx 'parsing' module
    eth_8_rx_t eth_rx = eth_8_rx(eth_rx_mac_axis_out, eth_rx_out_ready);
    stream(eth8_frame_t) frame = eth_rx.frame;

    // Filter out all but matching destination mac frames including broadcast ones
    uint1_t mac_match = frame.data.header.dst_mac == FPGA_MAC || frame.data.header.dst_mac == BROADCAST_MAC;

    // Match Ethertype to identify the protocol
    uint1_t arp_match = frame.data.header.ethertype == ETHERTYPE_ARP;
    // Write DVR handshake if mac match and protocol == ARP
    uint1_t valid_and_ready = frame.valid & eth_rx_out_ready & mac_match & arp_match;

    // Deserialize and connect to work pipeline input
    stream(axis8_t) deser_in_stream;
    deser_in_stream.data = frame.data.payload;
    deser_in_stream.valid = valid_and_ready;
    uint1_t deser_output_ready = arp_pipeline_in_ready;
    arp_deserialize_t deser = arp_deserialize(
        deser_in_stream,
        deser_output_ready);
    deser_ready_for_input = deser.packet_ready; // FEEDBACK
    arp_pipeline_in.data = deser.data;
    arp_pipeline_in.valid = deser.valid;

    // Header only written once at the start of data packet
    loopback_headers_fifo_in.data = frame.data.header;
    loopback_headers_fifo_in.valid = frame.data.start_of_payload & valid_and_ready;
}

////////////////////////////////////////////////////////////////////////////////
//      _________   __
//     |__   __\ \ / /
//        | |   \ V /
//        | |    > <
//        | |   / . \
//        |_|  /_/ \_\
//
////////////////////////////////////////////////////////////////////////////////

// TODO: if nothing is returned from ARP do not send a packet
MAIN_MHZ(tx_main, PLL_CLK_MHZ)
void tx_main() {
    // Wire up the ETH frame to send
    stream(eth8_frame_t) frame;
    // Header matches what was sent other than SRC+DST macs
    frame.data.header = loopback_headers_fifo_out.data;
    frame.data.header.dst_mac = frame.data.header.src_mac; // Send back to where came from
    frame.data.header.src_mac = FPGA_MAC;                  // From FPGA

    // Serialize results coming out of work pipeline
    uint1_t ser_output_ready;
    #pragma FEEDBACK ser_output_ready
    arp_serialize_t ser = arp_serialize(
        arp_pipeline_out.data,
        arp_pipeline_out.valid,
        ser_output_ready);

    // Header and serializer payload need to be valid to send
    arp_pipeline_out_ready = ser.in_data_ready;
    frame.data.payload.tdata = ser.out_data;
    frame.data.payload.tlast = ser.last;
    frame.valid = ser.valid & loopback_headers_fifo_out.valid;

    // The tx 'building' module
    eth_8_tx_t eth_tx = eth_8_tx(frame, eth_tx_mac_input_ready);
    eth_tx_mac_axis_in = eth_tx.mac_axis;

    // Read DVR handshake from inputs
    uint1_t valid_and_ready = frame.valid & eth_tx.frame_ready;

    // Read payload from serializer if was ready
    ser_output_ready = valid_and_ready; // FEEDBACK

    // Read header if was ready at end of packet
    loopback_headers_fifo_out_ready = frame.data.payload.tlast & valid_and_ready;
}