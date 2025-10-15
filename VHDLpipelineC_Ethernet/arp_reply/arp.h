// Handle ARP packages

// This describes the arp to be done:
// 	Input data format
// 	Output data format

#pragma once

#include "intN_t.h"

#ifdef __PIPELINEC__
#include "stream/stream.h"
#endif

#define ETHERTYPE_ARP       0x0806

#define ARP_HARDWARE_TYPE   0x0100  // Hardware type (e.g., Ethernet)   0x0001  bytes swapped
#define ARP_PROTOCOL_TYPE   0x0008  // Protocol type (e.g., IPv4)       0x0800  bytes swapped
#define ARP_HARDWARE_SIZE   6       // Size of hardware address (MAC address size)
#define ARP_PROTOCOL_SIZE   4       // Size of protocol address (IPv4 address size)

#define ARP_OPCODE_REQUEST  0x0100       // ARP request 1   bytes swapped
#define ARP_OPCODE_REPLY    0x0200       // ARP reply   2   bytes swapped

typedef struct arp_request_t
{
    uint16_t hardware_type;         // Hardware type
    uint16_t protocol_type;         // Protocol type
    uint8_t hardware_size;          // Length of hardware address
    uint8_t protocol_size;          // Length of protocol address
    uint16_t operation;             // Operation: request or reply
    uint48_t sender_hardware_addr;  // Sender MAC address
    uint32_t sender_protocol_addr;  // Sender IP address
    uint48_t target_hardware_addr;  // Target MAC address
    uint32_t target_protocol_addr;  // Target IP address
} arp_request_t;

// TODO: maybe there is a nicer way, than copying the struct here again. But at
//          least, this does work with PipelineC

#ifdef __PIPELINEC__
    #include "arp_request_t_bytes_t.h"
    DECL_STREAM_TYPE(arp_request_t)
#else
    #include "type_bytes_t.h/arp_request_t_bytes_t.h/arp_request_t_bytes.h"
#endif

typedef struct arp_reply_t
{
    uint16_t hardware_type;         // Hardware type
    uint16_t protocol_type;         // Protocol type
    uint8_t hardware_size;          // Length of hardware address
    uint8_t protocol_size;          // Length of protocol address
    uint16_t operation;             // Operation: request or reply
    uint48_t sender_hardware_addr;  // Sender MAC address
    uint32_t sender_protocol_addr;  // Sender IP address
    uint48_t target_hardware_addr;  // Target MAC address
    uint32_t target_protocol_addr;  // Target IP address
} arp_reply_t;

#ifdef __PIPELINEC__
    #include "arp_reply_t_bytes_t.h"
    DECL_STREAM_TYPE(arp_reply_t)
#else
    #include "type_bytes_t.h/arp_reply_t_bytes_t.h/arp_reply_t_bytes.h"
#endif

// build the arp reply packet based on the request data
// TODO: need to check the target IP address in the request
arp_reply_t arp(arp_request_t req)
{
    arp_reply_t reply;

    reply.hardware_type = ARP_HARDWARE_TYPE;
    reply.protocol_type = ARP_PROTOCOL_TYPE;
    reply.hardware_size = ARP_HARDWARE_SIZE;
    reply.protocol_size = ARP_PROTOCOL_SIZE;

    reply.operation = ARP_OPCODE_REPLY;

    reply.target_hardware_addr = req.sender_hardware_addr;
    reply.target_protocol_addr = req.sender_protocol_addr;

    reply.sender_hardware_addr = FPGA_MAC;
    reply.sender_protocol_addr = FPGA_IP;

    return reply;
}
