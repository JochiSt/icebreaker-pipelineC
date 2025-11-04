# general imports
import os

################################################################################
# SCAPY imports
import scapy.all as scapy

################################################################################
# cocotb imports
import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge
from cocotb.result import TestSuccess

# cocotb extension imports
from cocotbext.eth import GmiiFrame, RmiiPhy

################################################################################
# list all signals
#       from slide 33 of
#           https://github.com/lukedarnell/cocotb/blob/master/tests/test_cases/orconf2018/cocotb_orconf2018.pdf
#@cocotb.test()
async def list_signals(dut):
    for design_element in dut:
        dut._log.info("Found %s : python type = %s:"%(design_element, type(design_element)))
        dut._log.info("         : _name = %s"%(design_element._name))
        dut._log.info("         : _path = %s"%(design_element._path))

    raise TestSuccess

################################################################################
# test proper packet receiving
@cocotb.test()
async def test_eth_packet_RX(dut):

    # generate a clock
    pll_clock = Clock(dut.pll_clk, round(1/25e6*1e9,3), units="ns")
    clock_50MHz = Clock(dut.P1A9, round(1/50e6*1e9,3), units="ns")
    ice40_clock = Clock(dut.clk, round(1/12e6*1e9,3), units="ns")

    # connect RMII to (virtual) Ethernet PHY
    RMII_CLK_WIRE = dut.P1A9
    RMII_CRS_DV_WIRE = dut.P1A3
    RMII_TX_EN_WIRE = dut.P1A7

    eth_rst = None
    eth_rx_err = None

    rmii_phy = RmiiPhy(
        dut.eth_tx_vect,
        RMII_TX_EN_WIRE,
        RMII_CLK_WIRE,
        dut.eth_rx_vect,
        eth_rx_err,
        RMII_CRS_DV_WIRE,
        eth_rst,
        speed=100e6
        )

    # Start the clock. Start it low to avoid issues on the first RisingEdge
    cocotb.start_soon(pll_clock.start(start_high=False))
    cocotb.start_soon(ice40_clock.start(start_high=False))
    cocotb.start_soon(clock_50MHz.start(start_high=False))

    dut.pll_locked.value = 0    # disable PLL
    # reset the module, wait 2 rising edges until we release reset
    for _ in range(2):
        await RisingEdge(dut.pll_clk)
    dut.pll_locked.value = 1    # enable PLL

    ############################################################################
    # scapy -> cocotb
    # send an ARP request packet
    arp_packet = scapy.Ether() / scapy.ARP(pdst="192.168.0.20")

    await rmii_phy.rx.send(GmiiFrame.from_payload( scapy.raw(arp_packet)) )
    tx_data = await rmii_phy.tx.recv( )

    ############################################################################
    # cocotb -> scapy

    # Dissect the packet
    packet = scapy.Ether(bytes(tx_data.get_payload()))

    # Display the summary of each layer in the packet
    packet.show()


    # wait some time
    for _ in range(20):
        await RisingEdge(dut.pll_clk)


################################################################################
from cocotb.runner import get_runner
from pathlib import Path

def test_eth_layer_runner():
    # get environment variable or default
    sim = os.getenv("SIM", "ghdl")

    # get path to this file
    proj_path = Path(__file__).resolve().parent
    MODULES_FOLDER = proj_path / "../../"

    ############################################################################
    # define verilog sources
    verilog_sources = [
    ]

    ############################################################################
    # define VHDL sources
    vhdl_sources = [
        MODULES_FOLDER / "sim_top.vhdl",
    ]
    # readin sources from PipelineC output
    with open(MODULES_FOLDER / "pipelinec_output" / "vhdl_files.txt") as f:
        vhdl_sources += f.readline().split()

    ############################################################################
    # build the runner
    runner = get_runner(sim)
    runner.build(
        verilog_sources=verilog_sources,
        vhdl_sources=vhdl_sources,
        # build arguments for GHDL
        build_args=[
            "--std=08",
            "-frelaxed",
            "-Wno-unhandled-attribute",
            "-Wno-hide",
            ],
        hdl_toplevel="sim_top",
        always=False,   # build always?
        timescale=("1ns", "1ps"),
        waves=True,
    )

    ############################################################################
    # execute the test
    runner.test(hdl_toplevel="sim_top",
                test_module="test_eth_layer",
                waves=True,         # store traces
                parameters={
                    },
                plusargs =  [
                    "--wave=test_eth_layer.ghw", # store waves into file
                    ],
                )

if __name__ == "__main__":
    test_eth_layer_runner()