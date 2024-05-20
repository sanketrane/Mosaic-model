#!/bin/sh
#
module load oneapi/2024.0.0/2024.0.0
export PATH=/insomnia001/shared/apps/oneapi/2024.0/bin/compiler:$PATH
# export LD_LIBRARY_PATH=/insomnia001/shared/apps/oneapi/2024.0/lib:$LD_LIBRARY_PATH
# export LD_LIBRARY_PATH=/insomnia001/shared/apps/oneapi/2024.0/lib/libtbb.so.12:$LD_LIBRARY_PATH

source ~/.bashrc

# Create an array to hold the object file names
object_files=()

# Loop over all .cpp files in the current directory
for cpp_file in *.cpp
do
    # Compile the .cpp file into an .o file
    object_file="${cpp_file%.cpp}.o"
    icpx -c "$cpp_file" -o "$object_file" -tbb
    # Add the .o file to the array
    object_files+=("$object_file")
done

# Link all .o files into an executable
icpx "${object_files[@]}" -o mosaic_program -tbb

# End of script