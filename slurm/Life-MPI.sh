#!/bin/bash

#SBATCH --partition debug
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=4
#SBATCH --nodes=1
#SBATCH --time 00:10:00
#SBATCH --job-name LIFE-C
#SBATCH --output Life-MPI-OUTPUT.out
#SBATCH --mail-user dunharrow@unm.edu

module load openmpi
module load gcc


cd $SLURM_SUBMIT_DIR
mpicc ../c/gol_parralel.c -o  ../c/life2.out

srun  -n 4  ../c/gol_parallel 32 10

