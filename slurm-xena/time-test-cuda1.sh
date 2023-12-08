#!/bin/bash

#SBATCH --partition=singleGPU
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --time=20:00:00
#SBATCH --job-name PolyEvoP
#SBATCH --gpus=1
#SBATCH --output outputs/cuda-1procs.out

source ~/.bashrc

module load gcc/10.2.0-3kjq
module load cuda/11.4.2-rgi3
module load cmake/3.26.3-vyco
module load slurm/20.02.05-hfsl
module load openmpi/4.1.4-7gqe

nvidia-smi

cd cuda
srun -n 1 ./cuda-output 64 1000
srun -n 1 ./cuda-output 64 1000
srun -n 1 ./cuda-output 128 1000
srun -n 1 ./cuda-output 256 1000
srun -n 1 ./cuda-output 512 1000
srun -n 1 ./cuda-output 1024 1000
srun -n 1 ./cuda-output 2048 1000
srun -n 1 ./cuda-output 4096 1000
srun -n 1 ./cuda-output 8192 1000
srun -n 1 ./cuda-output 16384 1000
srun -n 1 ./cuda-output 32768 1000
srun -n 1 ./cuda-output 65536 1000
srun -n 1 ./cuda-output 131072 1000


