
PROJECT=VHDLtest
CONSTRAINT=icebreaker
TOP_MODULE=main
OBJECT_FILES=main.o

GHDL_OPTIONS=--latches

###############################################################################

all: $(PROJECT).rpt $(PROJECT).bin

prog: $(PROJECT).bin
	iceprog $<

clean:
	rm -f *.o *.cf

###############################################################################

$(PROJECT).json: $(OBJECT_FILES)
	yosys -m ghdl -p 'ghdl $(GHDL_OPTIONS) $(TOP_MODULE); synth_ice40 -json $@'

$(PROJECT).asc: $(PROJECT).json $(CONSTRAINT).pcf
	nextpnr-ice40 --up5k --package sg48 --pcf $(CONSTRAINT).pcf --freq 12 --asc $@ --json $<

$(PROJECT).bin: $(PROJECT).asc
	icepack $< $@

$(PROJECT).rpt: $(PROJECT).asc
	icetime -d up5k -c 12 -mtr $@ $<

###############################################################################

%.o: %.vhdl
	ghdl -a $<

