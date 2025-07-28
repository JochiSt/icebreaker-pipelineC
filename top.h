#include "uintN_t.h"
#pragma PART "ICE40UP5K-SG48" // TODO move into board/.h file?

// Get clock rate constant PLL_CLK_MHZ from header written by make flow
#include "pipelinec_makefile_config.h"

// By default PipelineC names clock ports with the rate included
// ex. clk_12p0
// Override this behavior by creating an input with a constant name
// and telling the tool that input signal is a clock of specific rate
#include "compiler.h"
DECL_INPUT(uint1_t, pll_clk)
CLK_MHZ(pll_clk, PLL_CLK_MHZ)
DECL_INPUT(uint1_t, pll_locked)


// Instantiate UART MAC module
#ifdef DEFAULT_PI_UART
#define UART_CLK_MHZ PLL_CLK_MHZ
#define UART_BAUD 115200
#include "uart/uart_mac.c"
#endif
