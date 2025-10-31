#pragma once

#include "intN_t.h"
#include "arrays.h"
#include "axi/axis.h"
#include "stream/stream.h"
#include "net/eth_8.h"

#include "fpga_mac.h"

////////////////////////////////////////////////////////////////////////////////
// Ethertypes
#define ETHERTYPE_ARP       0x0806
#define ETHERTYPE_IP        0x0800

// defines for ARP packet handling
#define ARP_HARDWARE_TYPE   0x0100  // Hardware type (e.g., Ethernet)   0x0001  bytes swapped
#define ARP_PROTOCOL_TYPE   0x0008  // Protocol type (e.g., IPv4)       0x0800  bytes swapped
#define ARP_HARDWARE_SIZE   6       // Size of hardware address (MAC address size)
#define ARP_PROTOCOL_SIZE   4       // Size of protocol address (IPv4 address size)

#define ARP_OPCODE_REQUEST  0x0100       // ARP request 1   bytes swapped
#define ARP_OPCODE_REPLY    0x0200       // ARP reply   2   bytes swapped

// defines for IP packet handling


////////////////////////////////////////////////////////////////////////////////
// Multiple outputs as a struct
typedef struct my_func_out_t {
    //  Output .data and .valid stream
    stream(eth8_frame_t) toIP;
    stream(eth8_frame_t) toARP;
    //  Output ready for input axis stream
    uint1_t ready_ethernet_in;
} my_func_out_t;

// ETHERNET Level packet parser
my_func_out_t parser_ethernet(stream(eth8_frame_t) input, uint1_t toARP_ready, uint1_t toIP_ready) {

    static my_func_out_t outputs; // Default value all zeros
    static uint1_t data_pending_ARP = 0;
    static uint1_t data_pending_IP = 0;

    // if we have no new data -> ready for new data
    outputs.ready_ethernet_in = ~ ( data_pending_ARP | data_pending_IP);

    ////////////////////////////////////////////////////////////////////////////
    // Input ready writes buffer and do some calculations
    if (input.valid) {
        // first look for matching MAC adresses
        uint1_t mac_match = input.data.header.dst_mac == FPGA_MAC || input.data.header.src_mac == BROADCAST_MAC;

        // look for ETHERTYPE
        uint1_t arp_match = input.data.header.ethertype == ETHERTYPE_ARP;
        uint1_t ip_match = input.data.header.ethertype == ETHERTYPE_IP;

        if(mac_match && arp_match){
            data_pending_ARP = 1;
            outputs.toARP = input;
            outputs.toARP.valid = 0;
        }else if(mac_match && ip_match){
            data_pending_IP = 1;
            outputs.toIP = input;
            outputs.toIP.valid = 0;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Output ready marks, that we are ready for new data
    if (data_pending_ARP) {
        if (toARP_ready) {
            data_pending_ARP = 0; // data has been read out
        }
        outputs.toARP.valid = 1;
    } else {
        outputs.toARP.valid = 0;
    }

    ////////////////////////////////////////////////////////////////////////////
    if (data_pending_IP) {
        if (toIP_ready) {
            data_pending_IP = 0; // data has been read out
        }
        outputs.toIP.valid = 1;
    } else {
        outputs.toIP.valid = 0;
    }

    return outputs;
}