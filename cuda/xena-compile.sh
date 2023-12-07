module load gcc/10.2.0-3kjq
module load cuda/11.4.2-rgi3
module load cmake/3.26.3-vyco
module load slurm/20.02.05-hfsl
module load openmpi/4.1.4-7gqe


nvcc -ccbin=mpicxx -arch=sm_35  -o cuda-output time_life.cpp mpi_life.cpp cuda_life.cu cuda.cu
