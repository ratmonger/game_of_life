#!/bin/bash

#SBATCH --partition normal
#SBATCH --ntasks-per-node=8
#SBATCH --nodes=2
#SBATCH --time 00:00:40
#SBATCH --job-name naive-life
#SBATCH --output naive-life.out
#SBATCH --mail-user dunharrow@unm.edu

module load openmpi
module load gcc

cd $SLURM_SUBMIT_DIR

srun -n 16  ../cpp/output 32 150

