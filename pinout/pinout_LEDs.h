#pragma once
#include "compiler.h"
#include "uintN_t.h"
#include "intN_t.h"


// normal LEDs
DECL_OUTPUT(uint1_t, LEDR_N)
GLOBAL_OUT_WIRE_CONNECT(uint1_t, LEDR_N, LED1)

DECL_OUTPUT(uint1_t, LEDG_N)
GLOBAL_OUT_WIRE_CONNECT(uint1_t, LEDG_N, LED2)

// RGB LED
DECL_OUTPUT(uint1_t, LED_RED_N)
DECL_OUTPUT(uint1_t, LED_GRN_N)
DECL_OUTPUT(uint1_t, LED_BLU_N)

#define LED_R_WIRE LED_RED_N
#define LED_G_WIRE LED_GRN_N
#define LED_B_WIRE LED_BLU_N

#include "rgb_led/led_rgb_wires.c"


