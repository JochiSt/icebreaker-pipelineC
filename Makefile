
YOSYS = yosys
NEXTPNR = nextpnr-ice40
ICEPACK = icepack
ICEPLL = icepll
PIPELINEC = pipelinec

TOP_NAME ?= top
PIPELINEC_TOP_FILE ?= $(TOP_NAME).c
SV_TOP_FILE ?= $(TOP_NAME).sv

# Only one PLL producing one clock in design for now...
PLL_CLK_MHZ ?= 25.0
MAIN_CLK_MHZ ?= 12.0

# Default nextpnr targetting the PLL clock freq
NEXTPNR_FREQ ?= $(PLL_CLK_MHZ)
NEXTPNR_ARGS ?= --freq $(NEXTPNR_FREQ)

GHDL_ARGS += --std=08 -frelaxed
GHDL_ARGS += -Wno-unhandled-attribute
GHDL_ARGS += -Wno-hide

all: pipelinec gateware.bin

.PHONY: clean prog pipelinec

# Program FPGA
prog: gateware.bin
	iceprog $<

clean:
	$(RM) lextab.py
	$(RM) yacctab.py
	$(RM) pipelinec_makefile_config.h
	$(RM) -r pipelinec_output
	$(RM) pipelinec.log
	$(RM) pll.v
	$(RM) *.json *.asc *.bin *.uf2
	$(RM) yosys_stderr.log

gateware.bin: $(SV_TOP_FILE)
	$(ICEPLL) -q -i $(MAIN_CLK_MHZ) -o $(PLL_CLK_MHZ) -p -m -f pll.v
	$(YOSYS) -q -m ghdl -p "ghdl $(GHDL_ARGS) `cat pipelinec_output/vhdl_files.txt` -e pipelinec_top; read_verilog -sv $(SV_TOP_FILE) pll.v; synth_ice40 -top $(TOP_NAME) -json $*.json"
	$(NEXTPNR) -q --seed 0 --up5k --package sg48 --pcf icebreaker.pcf --json $*.json --asc $*.asc $(NEXTPNR_ARGS)
	$(ICEPACK) $*.asc $@

# Cant invoke pipelinec with preprocessor macros set
# https://github.com/JulianKemmerer/PipelineC/issues/56
# So instead echo a .h file right before running tool...
pipelinec:
	echo "#define PLL_CLK_MHZ $(PLL_CLK_MHZ)\n" > pipelinec_makefile_config.h
	echo "#define MAIN_CLK_MHZ $(MAIN_CLK_MHZ)\n" >> pipelinec_makefile_config.h
	$(PIPELINEC) $(PIPELINEC_TOP_FILE) --top pipelinec_top --out_dir pipelinec_output --comb --no_synth

.SUFFIXES: .v .sv .asc .bin .uf2
