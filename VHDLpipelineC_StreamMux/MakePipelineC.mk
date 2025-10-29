PIPELINEC = pipelinec
PIPELINEC_FOLDER = .


TOP_NAME ?= top
PIPELINEC_TOP_FILE ?= $(PIPELINEC_FOLDER)/$(TOP_NAME).c

all: pipelinec

.PHONY: pipelinec_clean pipelinec

pipelinec_clean:
	$(RM) lextab.py
	$(RM) yacctab.py
	$(RM) pipelinec_makefile_config.h
	$(RM) -r pipelinec_output
	$(RM) pipelinec.log
	$(RM) pll.v
	$(RM) *.json *.asc *.bin *.uf2
	$(RM) yosys_stderr.log

pipelinec:
	$(PIPELINEC) $(PIPELINEC_TOP_FILE) --top pipelinec_top --out_dir pipelinec_output --comb --no_synth

