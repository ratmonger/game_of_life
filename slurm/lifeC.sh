#!/bin/bash

#SBATCH --partition debug
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --nodes=1
#SBATCH --time 00:10:00
#SBATCH --job-name LIFE-C
#SBATCH --output lifeC.out
#SBATCH --mail-user dunharrow@unm.edu

module load openmpi
module load gcc


cd $SLURM_SUBMIT_DIR
gcc ../c/game_of_life.c -o ../c/life.out

srun  -n 1  ../c/life.out 10 10 8
