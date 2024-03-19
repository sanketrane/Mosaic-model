#!/bin/bash

# Start the timer
start=$(date +%s)

# Run the Mosaic program and get the memory usage
/usr/bin/time ./mosaic_program 2>&1 | tee mosaic_output.txt

# End the timer
end=$(date +%s)

# Calculate the time taken
time_taken=$((end-start))

# Get memory usage in pages
memory_usage_pages=$(vm_stat | awk '/Pages active/ {print $3}' | sed 's/\.//')

# Convert memory usage to MB
memory_usage_mb=$((memory_usage_pages * 4096 / 1024 / 1024))

# Send an email to the user
echo -e "End of Mosaic run. Time taken: $time_taken seconds\nMemory usage: $memory_usage_mb MB" | mail -s "Mosaic Program Output" sanket.rane@columbia.edu
