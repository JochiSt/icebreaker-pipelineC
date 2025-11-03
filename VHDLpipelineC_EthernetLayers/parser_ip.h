/*******************************************************************************
 * Ethernet Layer protocol parser
 *
 * input:
 *      ip packet
 *
 * output:
 *      ethernet packet but splitted into
 */

#pragma once

#include "intN_t.h"
#include "arrays.h"
#include "axi/axis.h"
#include "stream/stream.h"
#include "net/eth_8.h"

#include "fpga_mac.h"

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Multiple outputs as a struct
typedef struct parser_ip_out_t {
    //  Output .data and .valid stream
    stream(axis8_t) toICMP;
    stream(axis8_t) toUDP;
    //  Output ready for input axis stream
    uint1_t ready_ethernet_in;
} parser_ip_out_t;

// ETHERNET Level packet parser
parser_ip_out_t parser_ip(stream(axis8_t) input, uint1_t toICMP_ready, uint1_t toUDP_ready) {

    static parser_ip_out_t outputs; // Default value all zeros
    static uint1_t data_pending_ICMP = 0;
    static uint1_t data_pending_UDP = 0;

    // if we have no new data -> ready for new data
    outputs.ready_ethernet_in = ~ ( data_pending_ICMP | data_pending_UDP);

    ////////////////////////////////////////////////////////////////////////////
    // Input ready writes buffer and do some calculations
    if (input.valid) {
        // first look for matching IP adress

        uint1_t mac_match = input.data.header.dst_mac == FPGA_MAC || input.data.header.src_mac == BROADCAST_MAC;

        // look for ETHERTYPE
        uint1_t arp_match = input.data.header.ethertype == ETHERTYPE_ARP;
        uint1_t ip_match = input.data.header.ethertype == ETHERTYPE_IP;

        if(mac_match && arp_match){
            data_pending_ICMP = 1;
            outputs.toICMP = input;
            outputs.toICMP.valid = 0;
        }else if(mac_match && ip_match){
            data_pending_UDP = 1;
            outputs.toUDP.data = input.data.payload;
            outputs.toUDP.valid = 0;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Output ready marks, that we are ready for new data
    if (data_pending_ICMP) {
        if (toICMP_ready) {
            data_pending_ICMP = 0; // data has been read out
        }
        outputs.toICMP.valid = 1;
    } else {
        outputs.toICMP.valid = 0;
    }

    ////////////////////////////////////////////////////////////////////////////
    if (data_pending_UDP) {
        if (toUDP_ready) {
            data_pending_UDP = 0; // data has been read out
        }
        outputs.toUDP.valid = 1;
    } else {
        outputs.toUDP.valid = 0;
    }

    return outputs;
}