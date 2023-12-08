#!/bin/bash

#SBATCH --partition=singleGPU
#SBATCH --nodes=9
#SBATCH --ntasks-per-node=1
#SBATCH --time=20:00:00
#SBATCH --job-name L9
#SBATCH --gpus=9
#SBATCH --output outputs/cuda-9procs.out

source ~/.bashrc

module load gcc/10.2.0-3kjq
module load cuda/11.4.2-rgi3
module load cmake/3.26.3-vyco
module load slurm/20.02.05-hfsl
module load openmpi/4.1.4-7gqe

nvidia-smi

cd ../cuda
srun -n 9 ./cuda-output 96 1000
srun -n 9 ./cuda-output 96 1000
srun -n 9 ./cuda-output 192 1000
srun -n 9 ./cuda-output 384 1000
srun -n 9 ./cuda-output 768 1000
srun -n 9 ./cuda-output 1536 1000
srun -n 9 ./cuda-output 3072 1000
srun -n 9 ./cuda-output 6144 1000
srun -n 9 ./cuda-output 12288 1000
srun -n 9 ./cuda-output 24576 1000
srun -n 9 ./cuda-output 49152 1000
srun -n 9 ./cuda-output 98304 1000
srun -n 9 ./cuda-output 196608 1000


