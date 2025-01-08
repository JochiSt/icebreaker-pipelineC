PIPELINEC=$(shell which pipelinec)
PIPELINEC_INCDIR=$(shell dirname $(PIPELINEC))/../include

bcd: bcd.o sim_bcd.o
	$(CC) $(CFLAGS) $^ -o $@

bcd.o: ../../pipelineC/pipelineC_bcd/bcd.C
	$(CC) -I$(PIPELINEC_INCDIR) -c $<

%.o: %.c
	$(CC) -c $<