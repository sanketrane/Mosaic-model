#!/bin/bash

#SBATCH -o /opt/mesh/eigg/sanket/Mosaic/slurm_out/%A_%a.out
#SBATCH --error=/opt/mesh/eigg/sanket/Mosaic/slurm_out/%A_%a.err_out
#SBATCH --get-user-env
#SBATCH -J sanket_MoSAIC
#SBATCH -D /opt/mesh/eigg/sanket/Mosaic
#SBATCH --nodelist=eigg
#SBATCH --array=1-5
#SBATCH --cpus-per-task=4

# Run the Mosaic program 
sh sh_files/runMosaic.sh $SLURM_ARRAY_TASK_ID 

# Send an email to the user
#echo -e "End of parallel Mosaic sims. Time taken : $time_taken seconds\nMemory usage: $memory_usage" #| mail -s "Mosaic Program Output" sanket.rane@columbia.edu
 
# End of script
