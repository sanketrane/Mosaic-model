#!/bin/bash

# Start the timer
start=$(date +%s)

# Run the Mosaic program
#!/bin/bash

# Run the Mosaic program and get the memory usage
/usr/bin/time -v ./mosaic_program 2>&1 | tee mosaic_output.txt
./mosaic_program

# End the timer
end=$(date +%s)

# Calculate the time taken
time_taken=$((end-start))

# Get memory usage
memory_usage=$(free -m | awk 'NR==2{printf "%.2f%%", $3*100/$2 }')

# Send an email to the user
echo -e "End of Mosiac run. Time taken : $time_taken seconds\nMemory usage: $memory_usage" | mail -s "Mosaic Program Output" sanket.rane@columbia.edu