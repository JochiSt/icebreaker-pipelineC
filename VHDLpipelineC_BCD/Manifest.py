import os

action = "simulation"

sim_tool = "ghdl"
sim_top = "tb_pipelineC_average"
ghdl_opt = "--workdir=work -fsynopsys --std=08"

sim_post_cmd = "ghdl -r tb_pipelineC_average --stop-time=5ms --wave=tb_pipelineC_average.ghw"

incl_makefiles = [
]

files = [
    "bcd.tb.vhdl",
    "bcd_top.tb.vhdl",
    "counter.vhdl",
    ]

modules = {
    "local" : [
        "pipelineC_bcd",
        "pipelineC_led_blink_counter",
        "pipelineC_mux",
    ],
}

 