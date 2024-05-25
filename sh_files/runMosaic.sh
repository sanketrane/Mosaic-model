#!/bin/bash

# Start the timer
start=$(date +%s)

export LD_LIBRARY_PATH=/opt/intel/oneapi/2024.0/lib/

# Run the Mosaic program and get the memory usage
# Use taskset to limit the program to 4 cores
/usr/bin/time -v ./mosaic_program \
    --tFin 200.0 \
    --tInit 5.0 \
    --tStep 0.1 \
    --cloneUniverse 1000 \
    --kilossrate 0.3 \
    --thyinfluxrate 0.4 \
    --writeRes_counter 100 \
    --writeClonefreq_counter 500 \
    --resfilePath output_files/results_${SLURM_ARRAY_TASK_ID}.csv 

# End the timer
end=$(date +%s)

# Calculate the time taken
time_taken=$((end-start))

# Get memory usage
memory_usage=$(free -m | awk 'NR==2{printf "%.2f%%", $3*100/$2 }')


# print the time taken
echo "End of Mosaic run. Time taken : $time_taken seconds." 
