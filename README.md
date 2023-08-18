# pipelineC and IceBreaker
This repository implements a simple blink program on the [IceBreaker FPGA board](https://1bitsquared.com/products/icebreaker)
using [pipelineC](https://github.com/JulianKemmerer/PipelineC) and the GHDL + YOSYS + nextpnr open source toolchain.

## VHDL pipelineC Blink
The code for blinking the LED is inside the file `blink.c`, it gets translated into `VHDL` using `pipelineC`. `GHDL` uses these files
in combination with `YOSYS` to synthesize the hardware for the FPGA.
