#!/bin/bash

#SBATCH --partition normal
#SBATCH --ntasks-per-node=8
#SBATCH --nodes=8
#SBATCH --time 00:30:00
#SBATCH --job-name naive-life
#SBATCH --output naive-life.out
#SBATCH --mail-user dunharrow@unm.edu

module load openmpi
module load gcc

cd cpp
srun -n 64 ./output 131072 1000
