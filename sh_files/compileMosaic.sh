#!/bin/bash

# Load the oneAPI module
if [[ -z "${ONEAPI_ROOT}" ]]; then      # If the oneAPI module is not loaded
    source /opt/intel/oneapi/setvars.sh
fi

# Compile the program
#make clean   # Clean the previous build
make mosaic_program  
