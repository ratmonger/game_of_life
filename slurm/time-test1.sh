#!/bin/bash

#SBATCH --partition normal
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --nodes=1
#SBATCH --time 10:00:00
#SBATCH --job-name naive-life
#SBATCH --output outputs/naive-life1000ticks-1procs.out
#SBATCH --mail-user dunharrow@unm.edu

module load openmpi
module load gcc

cd cpp
srun -n 1 ./output 64 1000
srun -n 1 ./output 128 1000
srun -n 1 ./output 256 1000
srun -n 1 ./output 512 1000
srun -n 1 ./output 1024 1000
srun -n 1 ./output 2048 1000
srun -n 1 ./output 4096 1000
srun -n 1 ./output 8192 1000
srun -n 1 ./output 16384 1000
srun -n 1 ./output 32768 1000
srun -n 1 ./output 65536 1000
srun -n 1 ./output 131072 1000
