
PROJECT=VHDLbcd
TOP_MODULE=bcd_top

# filename prefix = VHDL top module
CSOURCES=bcd.c mux.c led_blink_counter.c

VHDLSOURCES+=LUT_7segment.vhdl
VHDLSOURCES+=counter.vhdl
VHDLSOURCES+=bcd_top.vhdl

# prefix of constraint file *.pcf
CONSTRAINT=icebreaker

GHDL_OPTIONS=--std=08

###############################################################################

.PHONY: all
all: $(PROJECT).bin $(PROJECT).rpt

.PHONY: prog
prog: $(PROJECT).bin
	iceprog $<

.PHONY: clean
clean:
	rm -f *.o *.cf

.PHONY: total_clean
total_clean: clean
	rm -f lextab.py yacctab.py
	rm -f $(PROJECT).asc
	rm -f $(PROJECT).bin
	rm -f $(PROJECT).rpt
	rm -f $(PROJECT).json
	rm -rf pipelinec_*
	rm -f *_files.txt
	rm -f *.svg

###############################################################################

vhdl_files.txt: $(CSOURCES)
	@echo Building C sources $^
	echo -n "" > vhdl_files.txt
	for cfile in $(^:.c=_files.txt) ; do \
        make $$cfile ; \
		echo "############################################################################"; \
    done
	python3 orderVHDLfiles.py -o $@ $(^:.c=_files.txt)

# rule for each 
%_files.txt : %.c
	echo Building $< $@
	pipelinec --top=$(<:.c=) --out_dir=pipelinec_$(<:.c=) $<
	sed -i -e 's/ /\n/g' pipelinec_$(<:.c=)/vhdl_files.txt 
	cp pipelinec_$(<:.c=)/vhdl_files.txt $@
	
.PHONY: makePipelineCVDHLsources
makePipelineCVDHLsources: vhdl_files.txt
	$(eval PIPELINECVHDLSOURCES+=$(shell cat vhdl_files.txt))

.PHONY: makeObjectFileList
makeObjectFileList: makePipelineCVDHLsources
	$(eval OBJECTFILELIST+=$(PIPELINECVHDLSOURCES:.vhd=.o))
	@make $(OBJECTFILELIST)

work-obj08.cf: makeObjectFileList $(VHDLSOURCES:.vhdl=.o)

$(PROJECT).json: work-obj08.cf
	yosys -m ghdl -p 'ghdl $(GHDL_OPTIONS) $(TOP_MODULE); synth_ice40 -json $@'

$(PROJECT).asc: $(PROJECT).json $(CONSTRAINT).pcf
	nextpnr-ice40 --up5k --package sg48 --pcf $(CONSTRAINT).pcf --freq 12 --asc $@ --json $< 

$(PROJECT).bin: $(PROJECT).asc
	icepack $< $@

$(PROJECT).rpt: $(PROJECT).asc
	icetime -d up5k -c 12 -mtr $@ $<

###############################################################################
# SIMULATION

.PHONY: sim
sim: work-obj08.cf bcd_top.tb.o
	ghdl -e $(GHDL_OPTIONS) bcd_top_tb
	ghdl -r $(GHDL_OPTIONS) bcd_top_tb --stop-time=10us --wave=bcd_top_tb.ghw

###############################################################################

%.o: %.vhdl
	ghdl -a $(GHDL_OPTIONS) $<

%.o: %.vhd
	ghdl -a $(GHDL_OPTIONS) $<

###############################################################################