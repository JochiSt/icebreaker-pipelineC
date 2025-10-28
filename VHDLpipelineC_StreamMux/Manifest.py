import os
import subprocess

################################################################################

action = "simulation"

sim_tool = "ghdl"
sim_top = "stream_mux_tb"
ghdl_opt = "--workdir=work -fsynopsys --std=08"

sim_pre_cmd = "mkdir -p work"
sim_post_cmd = "ghdl -r stream_mux_tb --stop-time=600ns --wave=stream_mux_tb.ghw"

incl_makefiles = [
    "MakePipelineC.mk",
    "../MakeView.mk"
]

modules = {
    "local" : [
    ],
}

files = [
    "../VHDLpipelineC_AXI2VHDL/axi_master.vhdl",
    "../VHDLpipelineC_AXI2VHDL/axi_slave.vhdl",
    "stream_mux_tb.vhdl",
    ]

################################################################################
# store full path of VHDL files
full_path_files = []

with open("pipelinec_output/vhdl_files.txt") as vhdlfile:
    for line in vhdlfile:
        full_path_files = full_path_files + line.split()

print("building the file array...")
for file in full_path_files:
    existing = False
    for s in files:
        if os.path.basename(file) in s:
            existing = True
            break

    if not existing:
        files.append(os.path.relpath(file, os.getcwd()))

## deactivate float_pkg
## or somehow get the float_pkg into vivado
#print("Deactivating the float package...")
#for file in full_path_files:
#    sed_call = ["sed", "-i", "s/use ieee.float_pkg.all;/-- use ieee.float_pkg.all;/g", file]
#    #print("Calling:", " ".join(sed_call))
#    subprocess.call(sed_call)
