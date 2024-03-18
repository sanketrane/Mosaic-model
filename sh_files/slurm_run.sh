#!/bin/sh
#
#SBATCH --account=iicd           # group account name
#SBATCH --job-name=MosaicProgram # The job name
#SBATCH -N 1                     # The number of nodes to request
#SBATCH -c 1                     # The number of cpu cores to use (up to 32 cores per server)
#SBATCH -o slurm_out/%j.%N.out
#SBATCH --error=slurm_out/%j.%N.err_out

 
# Start the timer
start=$(date +%s)

# Run the Mosaic program
srun mosaic_program 

# End the timer
end=$(date +%s)

# Calculate the time taken
time_taken=$((end-start))

# Send an email to the user
echo "End of Mosaic run. Time taken : $time_taken seconds" 
 
# End of script