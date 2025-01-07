import os

action = "simulation"

sim_tool = "ghdl"
sim_top = "bcd_top_tb"
ghdl_opt = "--workdir=work -fsynopsys --std=08"

sim_pre_cmd = "mkdir work"
sim_post_cmd = "ghdl -r bcd_top_tb --stop-time=5ms --wave=bcd_top_tb.ghw"

incl_makefiles = [
    "../MakeView.mk",
]

modules = {
    "local" : [
        "../../pipelineC",
        "../../src",
    ],
}

files = [
    "bcd_top.tb.vhdl",
    ]
