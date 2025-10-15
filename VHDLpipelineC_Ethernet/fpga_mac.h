#pragma once
// Network, big endian, byte order
// MAC0 is most signficant byte
// MAC5 is least signficant byte
// "0:1:2:3:4:5"
////////////////////////////////////////////////////////////////////////////////
// FPGA MAC address
#define FPGA_MAC0	0xA0
#define FPGA_MAC1	0xB1
#define FPGA_MAC2	0xC2
#define FPGA_MAC3	0xD3
#define FPGA_MAC4	0xE4
#define FPGA_MAC5	0xF5

uint8_t FPGA_MAC_BYTES[6] = {FPGA_MAC0, FPGA_MAC1, FPGA_MAC2, FPGA_MAC3, FPGA_MAC4, FPGA_MAC5};
#define FPGA_MAC uint8_array6_be(FPGA_MAC_BYTES)

// FPGA IP address
#define FPGA_IP3    192
#define FPGA_IP2    168
#define FPGA_IP1    0
#define FPGA_IP0    20
uint8_t FPGA_IP_BYTES[4] = {FPGA_IP0, FPGA_IP1, FPGA_IP2, FPGA_IP3};
#define FPGA_IP uint8_array4_be(FPGA_IP_BYTES)

////////////////////////////////////////////////////////////////////////////////
// Broadcast MAC address
#define BROADCAST_MAC0   0xFF
#define BROADCAST_MAC1   0xFF
#define BROADCAST_MAC2   0xFF
#define BROADCAST_MAC3   0xFF
#define BROADCAST_MAC4   0xFF
#define BROADCAST_MAC5   0xFF

uint8_t BROADCAST_MAC_BYTES[6] = {BROADCAST_MAC0, BROADCAST_MAC1, BROADCAST_MAC2, BROADCAST_MAC3, BROADCAST_MAC4, BROADCAST_MAC5};

#define BROADCAST_MAC uint8_array6_be(BROADCAST_MAC_BYTES)