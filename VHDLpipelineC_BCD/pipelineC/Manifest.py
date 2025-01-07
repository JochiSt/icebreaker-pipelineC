################################################################################
# Manifest.py for all pipelineC files / projects
################################################################################

import os
import subprocess

# folder, topmodule
filefolders = [
    ["pipelineC_bcd", "bcd"],
    ["pipelineC_led_blink_counter", "led_blink_counter"],
    ["pipelineC_mux", "mux"],
]

run_pipelinec = True   # run pipelineC ??

################################################################################
# store full path of VHDL files
full_path_files = []

for folder, topmodule in filefolders:
    if run_pipelinec:
        ########################################################################
        # remove the existing pipelinec folder
        clean_call = ["rm", "-rf", folder + "/pipelinec"]
        print("Calling:", " ".join(clean_call))
        subprocess.call(clean_call)

        ########################################################################
        # collect all C files for forwarding to pipelineC
        Cfiles = []
        try:
            with os.scandir(folder) as entries:
                for entry in entries:
                    if entry.name[-2:] == ".C":
                        Cfiles.append(folder+"/"+entry.name)
        except FileNotFoundError:
            pass
        
        ########################################################################
        PIPELINEC_CMD = "pipelinec"

        pipelineC_arguments = [
            #"--no_synth",           # do not synthesize the files
            "--out_dir="+folder+"/pipelinec",  # use constant output folder
            "--top="+topmodule,         # set the name of the TOP module
            ]

        pipelineC_call = [PIPELINEC_CMD]
        pipelineC_call = pipelineC_call + pipelineC_arguments
        pipelineC_call = pipelineC_call + Cfiles

        print("Calling:", " ".join(pipelineC_call))
        subprocess.run(pipelineC_call)
    
    ############################################################################
    # first make a list of the full / absolute path
    with open(folder+"/pipelinec/vhdl_files.txt") as vhdlfile:
        for line in vhdlfile:
            full_path_files = full_path_files + line.split()

    # full_path_files contains all files of all pipelineC files

print("")
print(full_path_files)
print("")

print("building the file array...")
files = []
for file in full_path_files:
    existing = False
    for s in files:
        if os.path.basename(file) in s:
            existing = True
            break
    
    if not existing:
        files.append(os.path.relpath(file, os.getcwd()))          

# deactivate float_pkg
# or somehow get the float_pkg into vivado
print("Deactivating the float package...")
for file in full_path_files:
    sed_call = ["sed", "-i", "s/use ieee.float_pkg.all;/-- use ieee.float_pkg.all;/g", file]
    #print("Calling:", " ".join(sed_call))        
    subprocess.call(sed_call)

if __name__ == "__main__":
    for file in files:
        print(file)
    pass