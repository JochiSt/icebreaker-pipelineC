#pragma once

#include <stdint.h>
#include <stdlib.h>

#include <net/if.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>

#ifndef DEFAULT_IF
#define DEFAULT_IF "enx000ec6d9883d"
#endif

#define BUF_SIZ 1024
#define PAYLOAD_MAX (BUF_SIZ - sizeof(struct ether_header))
#define ETHER_TYPE 0xFFFF

void init_eth();
void close_eth();
void eth_read(uint8_t *payload_buf, size_t *payload_size);
void eth_write(uint8_t *payload_buf, size_t payload_size);
