#!/bin/bash

#SBATCH --partition normal
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=4
#SBATCH --nodes=1
#SBATCH --time 20:00:00
#SBATCH --job-name 4naive-life
#SBATCH --output outputs/naive-life1000ticks-4procs-other.txt
#SBATCH --mail-user dunharrow@unm.edu

module load openmpi
module load gcc

cd cpp
srun -n 4 ./output 64 1000
srun -n 4 ./output 64 1000
srun -n 4 ./output 128 1000
srun -n 4 ./output 256 1000
srun -n 4 ./output 512 1000
srun -n 4 ./output 1024 1000
srun -n 4 ./output 2048 1000
srun -n 4 ./output 4096 1000
srun -n 4 ./output 8192 1000
srun -n 4 ./output 16384 1000
srun -n 4 ./output 32768 1000
