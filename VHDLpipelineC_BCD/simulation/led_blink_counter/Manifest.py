import os

action = "simulation"

sim_tool = "ghdl"
sim_top = "led_blink_counter_tb"
ghdl_opt = "--workdir=work -fsynopsys --std=08"

sim_pre_cmd = "mkdir -p work"
sim_post_cmd = "ghdl -r led_blink_counter_tb --stop-time=2000ms --wave=led_blink_counter_tb.ghw"

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
    "led_blink_counter.tb.vhdl",
    ]
