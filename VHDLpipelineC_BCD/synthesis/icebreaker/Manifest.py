import subprocess

action = "synthesis"

incl_makefiles = [
]

# select FPGA
syn_device = "ice40"
syn_grade = "3"
syn_package = "ff"

# set project name and top module
syn_top = "bcd_top"
syn_project = "bcd_top"

syn_tool = "icestorm"

syn_pre_project_cmd = ""
syn_post_bitstream_cmd = ""

# modules, which needed to be included
modules = {
    "local": [
        "../../pipelineC",
        "../../src",
    ],
}

# local files needed for synthesis
files = [
    "../constraint/icebreaker.pcf",
    "bcd_top.vhdl",
    ]
