module load gcc/10.2.0-3kjq
module load cuda/11.4.2-rgi3
module load cmake/3.26.3-vyco
module load slurm/20.02.05-hfsl
module load openmpi/4.1.4-7gqe


mpicxx -o serial.out serial.cpp

mpicxx -o output time_life.cpp mpi_life.cpp naive.cpp openmp.cpp concurrency.cpp concurrency_openmp.cpp concurrencyTestAll.cpp cuda.cpp
#srun --partition=normal --nodes=8 --ntasks-per-node=8 --ntasks=64 ./output 128 1000
