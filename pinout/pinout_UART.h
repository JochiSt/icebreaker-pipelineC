// These UARTs are not hard wired, but are first ones used in testing
// Requires extra micropython code to set up on RaspPi
/*
// UART0 as default
#define ICE_11_OUT
#define UART_TX_OUT_WIRE ICE_11
#define ICE_9_IN
#define UART_RX_IN_WIRE ICE_9
*/
// UART1 as default
#define ICE_23_OUT
#define UART_TX_OUT_WIRE ICE_23
#define ICE_26_IN
#define UART_RX_IN_WIRE ICE_26
#endif

// GPIO multiple use pins

#ifdef ICE_11_OUT
DECL_OUTPUT(uint1_t, ICE_11)
#endif
#ifdef ICE_9_IN
DECL_INPUT(uint1_t, ICE_9)
#endif
#ifdef ICE_23_OUT
DECL_OUTPUT(uint1_t, ICE_23)
#endif
#ifdef ICE_26_IN
DECL_INPUT(uint1_t, ICE_26)
#endif