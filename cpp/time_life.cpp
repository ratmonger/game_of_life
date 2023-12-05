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
    MPI_Init(&argc, &argv);

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

    // Calculate variables
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

    init_grid(dim, A);
    init_grid(dim, B);

    // Initialize matrices A and B 
    /*int first_i = rank_row*N;
    int first_j = rank_col;
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            A[i*n+j] = ((rank_row*n)+i)*N + (rank_col*n)+j+1;
            B[i*n+j] = ((rank_row*n)+i)*N + (rank_col*n)+j+1;
        }
    }
    */
    //double sum_C, total_sum_C;
    double start, end;




    // ticks, A, B, dim, sqnumprocs, rank row, rank col

    // Time Simple Method
    //mpi_matmat_simple(A, B, C, n, sq_num_procs, rank_row, rank_col);
    // POSSIBLY CALL FIRST TEST

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    //mpi_matmat_simple(A, B, C, n, sq_num_procs, rank_row, rank_col);
    // CALL 2ND TEST


    end = MPI_Wtime() - start;
    //sum_C = mat_sum(n, C);
    //MPI_Reduce(&sum_C, &total_sum_C, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&end, &start, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0) printf("Matrix dim: %lu, Procs: %d , Elapsed Time %e\n", N, num_procs,start);

    delete[] A;
    delete[] B;

    MPI_Finalize();
    return 0;
}
