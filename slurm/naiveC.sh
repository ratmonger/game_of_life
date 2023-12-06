#!/bin/bash

#SBATCH --partition normal
#SBATCH --ntasks-per-node=8
#SBATCH --nodes=8
#SBATCH --ntasks=64
#SBATCH --time 00:10:00
#SBATCH --job-name NAIVE-C
#SBATCH --output outputs/NAIVEC.txt
#SBATCH --mail-user dunharrow@unm.edu

module load openmpi
module load gcc

cd cpp
sh compile.sh

srun -n 64 ./output 128 1
