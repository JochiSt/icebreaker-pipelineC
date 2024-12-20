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

with open("vhdl_files.txt", "r") as infile:
    for line in infile:
        rel_path = os.path.relpath(line.strip())
        files.append(rel_path)

 