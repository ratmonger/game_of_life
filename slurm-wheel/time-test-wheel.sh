#!/bin/bash

#SBATCH --partition normal
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --nodes=1
#SBATCH --time 20:00:00
#SBATCH --job-name serial-naive-life
#SBATCH --output outputs/serial1000ticks-wheelv2.out
#SBATCH --mail-user dunharrow@unm.edu

module load openmpi
module load gcc

cd cpp
srun ./serial.out 64 1000
srun ./serial.out 64 1000
srun ./serial.out 128 1000
srun ./serial.out 256 1000
srun ./serial.out 512 1000
srun ./serial.out 1024 1000
srun ./serial.out 2048 1000
srun ./serial.out 4096 1000
srun ./serial.out 8192 1000
srun ./serial.out 16384 1000
srun ./serial.out 32768 1000
