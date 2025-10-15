// Handle ARP packages

// This describes the arp to be done:
// 	Input data format
// 	Output data format
// 	The actual computation 'arp' to be done
// Ideally in PipelineC/C code that can be compiled+checked in software easily.
#pragma once

#include "intN_t.h"

#ifdef __PIPELINEC__
#include "stream/stream.h"
#endif

#define ETHERTYPE_ARP       0x0806

// MATRIX MULT EXAMPLE
#define DIM 2
#define data_t int8_t

typedef struct arp_request_t
{
  data_t matrix0[DIM][DIM];
  data_t matrix1[DIM][DIM];
} arp_request_t;

#ifdef __PIPELINEC__
    #include "arp_request_t_bytes_t.h"
    DECL_STREAM_TYPE(arp_request_t)
#else
    #include "type_bytes_t.h/arp_request_t_bytes_t.h/arp_request_t_bytes.h"
#endif

typedef struct arp_reply_t
{
  data_t result[DIM][DIM];
} arp_reply_t;

#ifdef __PIPELINEC__
    #include "arp_reply_t_bytes_t.h"
    DECL_STREAM_TYPE(arp_reply_t)
#else
    #include "type_bytes_t.h/arp_reply_t_bytes_t.h/arp_reply_t_bytes.h"
#endif

// Basic PipelineC implementation without pointers
arp_reply_t arp(arp_request_t req)
{
    arp_reply_t reply;
    uint32_t i;
    uint32_t j;
    uint32_t k;
    for (i = 0; i < DIM; i+=1)
    {
        for (j = 0; j < DIM; j+=1)
        {
            reply.result[i][j] = 0;
            for (k = 0; k < DIM; k+=1)
            {
                reply.result[i][j] += req.matrix0[i][k] * req.matrix1[k][j];
            }
        }
    }
    return reply;
}
