module load gcc
module load openmpi


mpicxx -o serial.out serial.cpp

mpicxx -o output time_life.cpp mpi_life.cpp naive.cpp openmp.cpp concurrency.cpp concurrency_openmp.cpp
#srun --partition=normal --nodes=8 --ntasks-per-node=8 --ntasks=64 ./output 128 1000
