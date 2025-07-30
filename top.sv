module top(
    // 12 MHz clock
    inout CLK,
    // RS232
    inout RX,
    inout TX,

    // LEDs and Button
    inout BTN_N,
    inout LEDR_N,
    inout LEDG_N,

    // RGB LED Driver
    inout LED_RED_N,
    inout LED_GRN_N,
    inout LED_BLU_N,

    // SPI Flash
    inout FLASH_SCK,
    inout FLASH_SSB,
    inout FLASH_IO0,
    inout FLASH_IO1,
    inout FLASH_IO2,
    inout FLASH_IO3,

    // PMOD 1A
    inout P1A1 ,
    inout P1A2 ,
    inout P1A3 ,
    inout P1A4 ,
    inout P1A7 ,
    inout P1A8 ,
    inout P1A9 ,
    inout P1A10,

    // PMOD 1B
    inout P1B1 ,
    inout P1B2 ,
    inout P1B3 ,
    inout P1B4 ,
    inout P1B7 ,
    inout P1B8 ,
    inout P1B9 ,
    inout P1B10,

    // PMOD 2
    inout P2_1 ,
    inout P2_2 ,
    inout P2_3 ,
    inout P2_4 ,
    inout P2_7 ,
    inout P2_8 ,
    inout P2_9 ,
    inout P2_10
  );

  wire clk_None;
  assign clk_None = CLK;

  // CLK has a default 12MHz clock in pico and pico2
  // PLL instance to make a clock based on 12MHz
  wire pll_clk;
  wire pll_locked;
  pll pll_inst(
        .clock_in(CLK),
        .clock_out(pll_clk),
        .locked(pll_locked)
      );

  // PipelineC output HDL instance
  pipelinec_top pipelinec_inst(
                  // The pipelinec port names exactly match wires in this top level .sv
                  // so can use wildcard .* implicit port connection to automatically connect them
                  .*
                );
endmodule
