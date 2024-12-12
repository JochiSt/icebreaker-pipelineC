
PROJECT=VHDLbcd
CONSTRAINT=icebreaker

TOP_MODULE=bcd
SOURCES=bcd.c

GHDL_OPTIONS=--std=08

###############################################################################

all: $(PROJECT).rpt $(PROJECT).bin

prog: $(PROJECT).bin
	iceprog $<

clean:
	rm -f *.o *.cf

total_clean: clean
	rm -f lextab.py yacctab.py
	rm -f $(PROJECT).asc
	rm -f $(PROJECT).bin
	rm -f $(PROJECT).rpt
	rm -f $(PROJECT).json
	rm -rf pipelinec
	rm -f *.svg

###############################################################################

vhdl_files.txt: $(CSOURCES)
	pipelinec --out_dir=pipelinec $(CSOURCES)
	sed -i -e 's/ /\n/g' pipelinec/vhdl_files.txt 
	ln -sf pipelinec/vhdl_files.txt vhdl_files.txt

.PHONY: makePipelineCVDHLsources
makePipelineCVDHLsources: vhdl_files.txt
	$(eval PIPELINECVHDLSOURCES+=$(shell cat vhdl_files.txt | grep "pkg"))	# package first
	$(eval PIPELINECVHDLSOURCES+=$(shell cat vhdl_files.txt | grep "built_in" | grep -v "pkg"))
	$(eval PIPELINECVHDLSOURCES+=$(shell cat vhdl_files.txt | grep -v "built_in" | grep -v "top" | grep -v "pkg"))
	$(eval PIPELINECVHDLSOURCES+=$(shell cat vhdl_files.txt | grep "top"))	# ensure top module is at the end
	echo $(PIPELINECVHDLSOURCES)

.PHONY: makeObjectFileList
makeObjectFileList: makePipelineCVDHLsources
	$(eval OBJECTFILELIST+=$(PIPELINECVHDLSOURCES:.vhdl=.o))
	$(eval OBJECTFILELIST+=$(VHDLSOURCES:.vhdl=.o))
	$(eval OBJECTFILELIST=$(OBJECTFILELIST:.vhd=.o))
	@echo "\n\n\n\n\n"
	@echo $(OBJECTFILELIST)
	@echo "\n\n\n\n\n"
	@make $(OBJECTFILELIST)

work-obj08.cf: makeObjectFileList

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
	ghdl -a $(GHDL_OPTIONS) $<

%.o: %.vhd
	ghdl -a $(GHDL_OPTIONS) $<
