################################################################################
# order the VHDL files, such, that they are unique and ordered
# used to forward multiple pipelinec units within one VHDL project

import argparse
import os

parser = argparse.ArgumentParser(
                    prog='orderVHDLfiles.py',
                    description='Order and filter the VHDL files',
                    epilog='')

parser.add_argument('file', type=argparse.FileType('r'), nargs='+')
parser.add_argument('-o', '--output')

args = parser.parse_args()

package_files = []
built_in_files = []
module_files = []
main_files = []

for f in args.file:
    for line in f:
        line = line.split()[0]    # remove \n
        if "pkg" in line:
            if not any(os.path.basename(line) in s for s in package_files):
                package_files.append(line)
        elif "built_in" in line:
            if not any(os.path.basename(line) in s for s in built_in_files):
                built_in_files.append(line)
        else:
            # check whether we have a module
            if os.path.basename(line).split(".")[0] == os.path.dirname(line).split("/")[-1]:
                if not any(os.path.basename(line) in s for s in main_files):
                    main_files.append(line)
            else:    
                if not any(os.path.basename(line) in s for s in main_files):
                    module_files.append(line)
                
with open(args.output, "w") as outfile:
    for file in package_files:
        outfile.write("%s\n"%(file))
    for file in built_in_files:
        outfile.write("%s\n"%(file))
    for file in module_files:
        outfile.write("%s\n"%(file))
    for file in main_files:
        outfile.write("%s\n"%(file))          