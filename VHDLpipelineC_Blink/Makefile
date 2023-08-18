
PROJECT=VHDLtest
CONSTRAINT=icebreaker

TOP_MODULE=blink
SOURCES=blink.c

GHDL_OPTIONS=--latches --std=08

###############################################################################

all: $(PROJECT).rpt $(PROJECT).bin

prog: $(PROJECT).bin
	iceprog $<

clean:
	rm -f *.o *.cf

###############################################################################

work-obj08.cf: $(SOURCES)
	pipelinec --out_dir=pipelinec $(SOURCES)
	ghdl -a --std=08 pipelinec/c_structs_pkg.pkg.vhd
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_EQ_uint25_t_uint24_t/BIN_OP_EQ_uint25_t_uint24_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uint25_t_uint25_t/MUX_uint1_t_uint25_t_uint25_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/UNARY_OP_NOT_uint1_t/UNARY_OP_NOT_uint1_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uint1_t_uint1_t/MUX_uint1_t_uint1_t_uint1_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_PLUS_uint25_t_uint1_t/BIN_OP_PLUS_uint25_t_uint1_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/blink/blink_0CLK_a5a1cd4e.vhd
	ghdl -a --std=08 pipelinec/top/top.vhd

$(PROJECT).json: work-obj08.cf
	yosys -m ghdl -p 'ghdl $(GHDL_OPTIONS) top; synth_ice40 -json $@'

$(PROJECT).asc: $(PROJECT).json $(CONSTRAINT).pcf
	nextpnr-ice40 --up5k --package sg48 --pcf $(CONSTRAINT).pcf --freq 12 --asc $@ --json $<

$(PROJECT).bin: $(PROJECT).asc
	icepack $< $@

$(PROJECT).rpt: $(PROJECT).asc
	icetime -d up5k -c 12 -mtr $@ $<

###############################################################################

%.o: %.vhdl
	ghdl -a $<

