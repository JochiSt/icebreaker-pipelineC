
# create include directory for pipelinec specific header files
PIPELINEC=$(shell which pipelinec)
PIPELINEC_INCDIR=$(shell dirname $(PIPELINEC))/../include

# use gcc
CC=gcc

# setup compiler flags
CFLAGS+=-Wall 
CFLAGS+=-Werror 
CFLAGS+=-Wno-unknown-pragmas
CFLAGS+=-Wextra 
CFLAGS+=-pedantic 
# use C as language for files
CFLAGS+=-x c 
CFLAGS+=-std=gnu99

LDFLAGS+=

.PHONY: all
all: led_blink_counter

led_blink_counter: sim_led_blink_counter.o led_blink_counter.o
	$(CC) $(LDFLAGS) $^ -o $@

led_blink_counter.o: ../../pipelineC/pipelineC_led_blink_counter/led_blink_counter.C
	$(CC) $(CFLAGS) -I$(PIPELINEC_INCDIR) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -I$(PIPELINEC_INCDIR) -c $<

.PHONY: clean
clean:
	rm -f *.o
	rm -f led_blink_counter