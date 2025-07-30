// These UARTs are not hard wired, but are first ones used in testing

#define ICE_09_OUT
#define UART_TX_OUT_WIRE TX
#define ICE_06_IN
#define UART_RX_IN_WIRE RX

// GPIO multiple use pins
#ifdef ICE_09_OUT
DECL_OUTPUT(uint1_t, TX)
#endif
#ifdef ICE_06_IN
DECL_INPUT(uint1_t, RX)
#endif
