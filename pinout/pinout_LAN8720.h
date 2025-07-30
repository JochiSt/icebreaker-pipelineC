#pragma once
#include "compiler.h"
#include "uintN_t.h"

// Use RMII phy wires attached to icebreaker PMOD1A
//  LAN8720 board is not a real standard PMOD
//  so is inserted upside down in the PMOD connector
#define RMII_CLK_WIRE       P1A9

#define RMII_CRS_DV_WIRE    P1A3
#define RMII_RX0_WIRE       P1A8
#define RMII_RX1_WIRE       P1A2

#define RMII_TX_EN_WIRE     P1A7
#define RMII_TX0_WIRE       P1A1
#define RMII_TX1_WIRE       P1A10

DECL_INPUT(uint1_t, RMII_CLK_WIRE)

DECL_INPUT(uint1_t, RMII_CRS_DV_WIRE)
DECL_INPUT(uint1_t, RMII_RX0_WIRE)
DECL_INPUT(uint1_t, RMII_RX1_WIRE)

DECL_OUTPUT(uint1_t, RMII_TX_EN_WIRE)
DECL_OUTPUT(uint1_t, RMII_TX0_WIRE)
DECL_OUTPUT(uint1_t, RMII_TX1_WIRE)




