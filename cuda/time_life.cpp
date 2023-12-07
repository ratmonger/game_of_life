#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#include "mpi_life.hpp"

// Main Method : 
//     Splits processes into a process grid
//         - rank_row : row of process in process grid
//         - rank_col : column of process in process grid
//     Creates two local matrices, A, B
//     Prints timings of methods
int main(int argc, char* argv[])
{

    // Initialize MPI
    //MPI_Init(&argc, &argv);
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    // Get rank of process and number of processes
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Make sure matrix dimension is in argv
    if (argc <= 2)
    {   
        if (rank == 0)
            printf("Pass Matrix Dimension and Ticks as Command Line Argument!\n");
        MPI_Finalize();
        return 1;
    }

    // Grab global dimension of matrices (A, B)
    unsigned long N = atoi(argv[1]);
    int ticks = atoi(argv[2]);


    // Calculate how many process rows/cols in process-grid
    int sq_num_procs = sqrt(num_procs);
    if (sq_num_procs*sq_num_procs != num_procs)
    {
        if (rank == 0) 
            printf("Number of processes needs to be a square\n");
        MPI_Finalize();
        return 1;
    }

    // - rank_row : process row
    // - rank_col : process col
    // - n : local (per-process) matrix dimension
    int rank_row = rank / sq_num_procs;
    int rank_col = rank % sq_num_procs;
    unsigned long dim = N / sq_num_procs; // n is unpadded dimension
    
    unsigned long size = dim*dim;

    if (dim*dim*num_procs != N*N)
    {
        if (rank == 0) 
            printf("Cannot evenly split %lu rows and cols over %d processes\n",
                    N, num_procs);
        MPI_Finalize();
        return 1;
    }

    // Allocate two local matrices (A, B)
    char* A;
    char* B;

    A = agar(dim);
    B = init_grid(dim);

    double start, end;


    // first test: mpi naive
    MPI_Barrier(MPI_COMM_WORLD);

    start = MPI_Wtime();
    cuda_naive( A, B, dim, sq_num_procs, rank_row, rank_col, ticks, rank);
    end = MPI_Wtime() - start;

    MPI_Reduce(&end, &start, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0) printf("CUDA -- Matrix dim: %lu, Procs: %d , Elapsed Time %e\n", N, num_procs,start);

    free(A);
    free(B);


    MPI_Finalize();

    return 0;
}
