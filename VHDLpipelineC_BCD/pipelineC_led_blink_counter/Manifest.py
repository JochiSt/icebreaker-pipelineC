import os
import subprocess

# remove the existing pipelinec folder
clean_call = ["rm", "-rf", "pipelinec"]
subprocess.call(clean_call)

# colled all C files for forwarding to pipelineC
Cfiles = []
try:
    with os.scandir(".") as entries:
        for entry in entries:
            if entry.name[-2:] == ".C":
                Cfiles.append(entry.name)
except FileNotFoundError:
    pass

################################################################################
PIPELINEC_CMD = "pipelinec"

pipelineC_arguments = [
    "--no_synth",           # do not synthesize the files
    "--out_dir=pipelinec",  # use constant output folder
    "--top=signed_average", # set the name of the TOP module
    ]

pipelineC_call = [PIPELINEC_CMD]
pipelineC_call = pipelineC_call + pipelineC_arguments
pipelineC_call = pipelineC_call + Cfiles

print("Calling:", pipelineC_call)

# call the pipelineC util
subprocess.run(pipelineC_call)

# return the VHDL files
VHDLfilelist = "pipelinec/vhdl_files.txt"

# first make a list of the full / absolute path
full_path_files = []
with open(VHDLfilelist) as vhdlfile:
    for line in vhdlfile:
        full_path_files = full_path_files + line.split()

# deactivate float_pkg
# or somehow get the float_pkg into vivado
for file in full_path_files:
    sed_call = ["sed", "-i", "s/use ieee.float_pkg.all;/-- use ieee.float_pkg.all;/g", file]
    subprocess.call(sed_call)


# strip the relative path to this Manifest
files = []
for file in full_path_files:
    files.append(os.path.relpath(file, os.getcwd()))

if __name__ == "__main__":
    print(files)