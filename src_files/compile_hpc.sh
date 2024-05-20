#!/bin/sh

# Create an array to hold the object file names
object_files=()

# Loop over all .cpp files in the current directory
for cpp_file in *.cpp
do
    # Compile the .cpp file into an .o file
    object_file="${cpp_file%.cpp}.o"
    icpx -c "$cpp_file" -o "$object_file"
    # Add the .o file to the array
    object_files+=("$object_file")
done

# Link all .o files into an executable
icpx "${object_files[@]}" -o mosaic_program -ltbb

# End of script