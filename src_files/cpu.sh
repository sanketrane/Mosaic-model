#!/bin/sh
#
#SBATCH --account=iicd           # group account name
#SBATCH --job-name=MosaicProgram # The job name
#SBATCH -c 5                     # Number of nodes
#SBATCH --mem-per-cpu=8G       # Default is 5800
#SBATCH -o slurm_out/%j.%N.out
#SBATCH --error=slurm_out/%j.%N.err_out

module load oneapi/2024.0.0/2024.0.0
source /insomnia001/shared/apps/oneapi/2024.0/oneapi-vars.sh

# ./compile_hpc.sh

# Start the timer
start=$(date +%s)

./mosaic_program 450.0 5.0 0.1 0.23 0.4 output_files/result.csv 7.0 output_files/freq.csv 90.0

# End the timer
end=$(date +%s)

# Calculate the time taken
time_taken=$((end-start))

# Send an email to the user
echo "End of Mosaic run. Time taken : $time_taken seconds." 
 
# End of script
