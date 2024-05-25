#!/bin/sh

#SBATCH --account=iicd               # group account name
#SBATCH --job-name=SR_MosaicProgram  # The job name
#SBATCH -o slurm_out/%j.%N.out
#SBATCH --error=slurm_out/%j.%N.err_out
#SBATCH --array=1-5
#SBATCH --cpus-per-task=4

# Load the oneAPI module on the cluster
module load oneapi/2024.0.0/2024.0.0
source /insomnia001/shared/apps/oneapi/2024.0/oneapi-vars.sh

# Compile the program
make mosaic_program  

# Run the Mosaic program 
sh sh_files/runMosaic.sh $SLURM_ARRAY_TASK_ID 

# End of script
