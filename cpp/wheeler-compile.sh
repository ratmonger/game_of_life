module load gcc
module load openmpi

mpicxx -o output time_life.cpp mpi_life.cpp mpi_naive.cpp
