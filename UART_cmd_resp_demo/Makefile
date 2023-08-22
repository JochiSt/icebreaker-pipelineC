
PROJECT=VHDLtest
CONSTRAINT=icebreaker

TOP_MODULE=blink
SOURCES=uart_cmd_resp_demo.c

GHDL_OPTIONS=--latches --std=08

PWD=$(shell pwd)/

###############################################################################

all: $(PROJECT).rpt $(PROJECT).bin

prog: $(PROJECT).bin
	iceprog $<

clean:
	rm -f *.o *.cf

total_clean: clean
	rm -f lextab.py yacctab.py
	rm -f $(PROJECT).{asc,bin,rpt,json}
	rm -rf pipelinec

###############################################################################

pipelinec/$(SOURCES).parsed: $(SOURCES)
	pipelinec --out_dir=pipelinec $(SOURCES)

work-obj08.cf: pipelinec/$(SOURCES).parsed
	ghdl -a --std=08 pipelinec/c_structs_pkg.pkg.vhd
	ghdl -a --std=08 pipelinec/global_wires_pkg.pkg.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_EQ_uint1_t_uint1_t/BIN_OP_EQ_uint1_t_uint1_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_EQ_uint2_t_uint2_t/BIN_OP_EQ_uint2_t_uint2_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_EQ_uint4_t_uint4_t/BIN_OP_EQ_uint4_t_uint4_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_EQ_uint8_t_uint1_t/BIN_OP_EQ_uint8_t_uint1_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_EQ_uint32_t_uint4_t/BIN_OP_EQ_uint32_t_uint4_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_EQ_uint32_t_uint9_t/BIN_OP_EQ_uint32_t_uint9_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_AND_uint1_t_uint1_t/BIN_OP_AND_uint1_t_uint1_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_OR_uint1_t_uint1_t/BIN_OP_OR_uint1_t_uint1_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_PLUS_uint4_t_uint1_t/BIN_OP_PLUS_uint4_t_uint1_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_PLUS_uint16_t_uint1_t/BIN_OP_PLUS_uint16_t_uint1_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_PLUS_uint32_t_uint1_t/BIN_OP_PLUS_uint32_t_uint1_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/BIN_OP_GTE_uint16_t_uint16_t/BIN_OP_GTE_uint16_t_uint16_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uint1_t_uint1_t/MUX_uint1_t_uint1_t_uint1_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uint1_t_8_uint1_t_8/MUX_uint1_t_uint1_t_8_uint1_t_8_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uint8_t_9_uint8_t_9/MUX_uint1_t_uint8_t_9_uint8_t_9_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uint4_t_uint4_t/MUX_uint1_t_uint4_t_uint4_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uint8_t_uint8_t/MUX_uint1_t_uint8_t_uint8_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uint32_t_uint32_t/MUX_uint1_t_uint32_t_uint32_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uint16_t_uint16_t/MUX_uint1_t_uint16_t_uint16_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/UNARY_OP_NOT_uint1_t/UNARY_OP_NOT_uint1_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uart_rx_mac_state_t_uart_rx_mac_state_t/MUX_uint1_t_uart_rx_mac_state_t_uart_rx_mac_state_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uart_tx_mac_state_t_uart_tx_mac_state_t/MUX_uint1_t_uart_tx_mac_state_t_uart_tx_mac_state_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_uart_cmd_resp_t_uart_cmd_resp_t/MUX_uint1_t_uart_cmd_resp_t_uart_cmd_resp_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/MUX_uint1_t_bus_transactor_state_t_bus_transactor_state_t/MUX_uint1_t_bus_transactor_state_t_bus_transactor_state_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_0_uint8_t/CONST_SR_0_uint8_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_1_uint8_t/CONST_SR_1_uint8_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_2_uint8_t/CONST_SR_2_uint8_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_3_uint8_t/CONST_SR_3_uint8_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_4_uint8_t/CONST_SR_4_uint8_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_5_uint8_t/CONST_SR_5_uint8_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_6_uint8_t/CONST_SR_6_uint8_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_7_uint8_t/CONST_SR_7_uint8_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_0_uint4_t/CONST_SR_0_uint4_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_1_uint4_t/CONST_SR_1_uint4_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_2_uint4_t/CONST_SR_2_uint4_t_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/built_in/CONST_SR_3_uint4_t/CONST_SR_3_uint4_t_0CLK_de264c78.vhd
	#
	ghdl -a --std=08 pipelinec/leds/led0_3_ports.c/led0_module/led0_module_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/leds/led0_3_ports.c/led1_module/led1_module_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/leds/led0_3_ports.c/led2_module/led2_module_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/leds/led0_3_ports.c/led3_module/led3_module_0CLK_de264c78.vhd
	ghdl -a --std=08 pipelinec/leds/leds_port.c/leds_module/leds_module_0CLK_6f2c5aad.vhd
	#
	ghdl -a --std=08 pipelinec/uart/uart_mac.c/uart_serializer_serializer_in_to_out/uart_serializer_serializer_in_to_out_0CLK_73702545.vhd
	ghdl -a --std=08 pipelinec/uart/uart_mac.c/uart_serializer/uart_serializer_0CLK_e02a63ae.vhd
	ghdl -a --std=08 pipelinec/uart/uart_mac.c/uart_deserializer/uart_deserializer_0CLK_f2389995.vhd
	ghdl -a --std=08 pipelinec/uart/uart_mac.c/uart_tx_mac/uart_tx_mac_0CLK_e803f5b1.vhd
	#
	ghdl -a --std=08 pipelinec/uart/uart_mac.c/uart_rx_mac/uart_rx_mac_0CLK_b7ac2955.vhd
	#
	ghdl -a --std=08 pipelinec/type_bytes_t.h/uint8_t_bytes_t.h/bytes_to_uint8_t_0CLK_23f04728.vhd
	ghdl -a --std=08 pipelinec/type_bytes_t.h/uint32_t_bytes_t.h/bytes_to_uint32_t_0CLK_23eb2db7.vhd
	#
	ghdl -a --std=08 pipelinec/type_bytes_t.h/uint8_t_bytes_t.h/uint8_t_to_bytes_0CLK_23f04728.vhd
	ghdl -a --std=08 pipelinec/type_bytes_t.h/uint32_t_bytes_t.h/uint32_t_to_bytes_0CLK_a5a1cd4e.vhd
	#
	ghdl -a --std=08 pipelinec/uart/uart_ports.c/uart_module/uart_module_0CLK_380ecc95.vhd
	ghdl -a --std=08 pipelinec/cmd_deser_deserializer_in_to_out/cmd_deser_deserializer_in_to_out_0CLK_262c3538.vhd
	ghdl -a --std=08 pipelinec/type_bytes_t.h/uart_cmd_resp_t_bytes_t.h/bytes_to_uart_cmd_resp_t_0CLK_3cd7da70.vhd
	ghdl -a --std=08 pipelinec/type_bytes_t.h/uart_cmd_resp_t_bytes_t.h/uart_cmd_resp_t_to_bytes_0CLK_261e5476.vhd
	ghdl -a --std=08 pipelinec/cmd_deser/cmd_deser_0CLK_900b4c03.vhd
	ghdl -a --std=08 pipelinec/uart_cmd_deser/uart_cmd_deser_0CLK_f218cef0.vhd
	ghdl -a --std=08 pipelinec/resp_ser_serializer_in_to_out/resp_ser_serializer_in_to_out_0CLK_46d2aa1c.vhd
	ghdl -a --std=08 pipelinec/resp_ser/resp_ser_0CLK_9c8b6ebf.vhd
	ghdl -a --std=08 pipelinec/uart_resp_ser/uart_resp_ser_0CLK_40c14f7a.vhd
	ghdl -a --std=08 pipelinec/uart_cmd_deser/uart_cmd_deser_0CLK_f218cef0.vhd
	#
	ghdl -a --std=08 pipelinec/bus_transactor/bus_transactor_0CLK_fa355561.vhd
	#
	ghdl -a --std=08 pipelinec/top/top.vhd

$(PROJECT).json: work-obj08.cf
	yosys -m ghdl -p 'ghdl $(GHDL_OPTIONS) top; synth_ice40 -json $@'

$(PROJECT).asc: $(PROJECT).json $(CONSTRAINT).pcf
	nextpnr-ice40 --up5k --package sg48 --pcf $(CONSTRAINT).pcf --freq 12 --asc $@ --json $< --placed-svg $(PROJECT)_placed.svg --routed-svg $(PROJECT)_routed.svg

$(PROJECT).bin: $(PROJECT).asc
	icepack $< $@

$(PROJECT).rpt: $(PROJECT).asc
	icetime -d up5k -c 12 -mtr $@ $<

###############################################################################

%.o: %.vhdl
	ghdl -a $<

#sed 's///g'
