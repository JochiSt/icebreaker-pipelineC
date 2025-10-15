#pragma once

#include "uintN_t.h"
#include "arrays.h"
#include "bit_manip.h"
#include "stream/stream.h"
#include "axi/axis.h"
#include "net/eth.h"
#include "stream/stream.h"

#include "../fpga_mac.h"

#define ETHERTYPE_ARP       0x0806
#define ARP_HARDWARE_TYPE   0x0001  // Hardware type (e.g., Ethernet)
#define ARP_PROTOCOL_TYPE   0x0800  // Protocol type (e.g., IPv4)
#define ARP_HARDWARE_SIZE   6       // Size of hardware address (MAC address size)
#define ARP_PROTOCOL_SIZE   4       // Size of protocol address (IPv4 address size)

typedef struct arp_packet_t {
    uint16_t hardware_type;         // Hardware type
    uint16_t protocol_type;         // Protocol type
    uint8_t hardware_size;          // Length of hardware address
    uint8_t protocol_size;          // Length of protocol address
    uint16_t operation;             // Operation: request or reply
    uint48_t sender_hardware_addr;  // Sender MAC address
    uint32_t sender_protocol_addr;  // Sender IP address
    uint48_t target_hardware_addr;  // Target MAC address
    uint32_t target_protocol_addr;  // Target IP address
} arp_packet_t;

#define ARP_DATA_SIZE   28  // size in 8 bit of arp_packet_t

typedef arp_packet_t arp_request_t;
typedef arp_packet_t arp_reply_t;

#include "arp_request_t_bytes_t.h"
DECL_STREAM_TYPE(arp_request_t)

#include "arp_reply_t_bytes_t.h"
DECL_STREAM_TYPE(arp_reply_t)

// construct the ARP reply based on the ARP request data
arp_reply_t arp(arp_request_t request){
    arp_reply_t reply;

    reply.hardware_type = ARP_HARDWARE_TYPE;
    reply.protocol_type = ARP_PROTOCOL_TYPE;
    reply.hardware_size = ARP_HARDWARE_SIZE;
    reply.protocol_size = ARP_PROTOCOL_SIZE;

    reply.target_hardware_addr = request.sender_hardware_addr;
    reply.target_protocol_addr = request.sender_protocol_addr;

    reply.sender_hardware_addr = FPGA_MAC;
    reply.sender_protocol_addr = FPGA_IP;

    return reply;
}