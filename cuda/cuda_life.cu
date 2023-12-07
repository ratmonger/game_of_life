#include "mpi_life.hpp"

__global__ void updateKernel(unsigned long dim, char* A, char* B)
{
    int row = blockIdx.x*blockDim.x + threadIdx.x +1;
    int col = blockIdx.y*blockDim.y + threadIdx.y +1;


    //consider making a 1-dim block and let thread iterate across a single row of matrix?

    if (row < dim +1  && col < dim +1 && row > 0 && col > 0){
        unsigned long n;
        unsigned long padDim = dim + 2;
        int liveNeighbors;


        n = (row * (dim+2)) + col;

        liveNeighbors = A[n - 1] +
            A[n + 1] +
            A[n - (padDim)] +
            A[n + (padDim)] +
            A[n - (padDim) - 1]+
            A[n + (padDim) - 1]+
            A[n - (padDim) + 1]+
            A[n + (padDim) + 1];

        // alive
        if (A[n]) {
            if (liveNeighbors < 2 || liveNeighbors > 3) {
                B[n] = 0; // dead due to underpopulation or overpopulation
            } else {
                B[n] = 1; // survive to the next generation
            }
        } else { // dead
            if (liveNeighbors == 3) {
                B[n] = 1; // becomes alive due to reproduction
            } else {
                B[n] = 0;   // stay dead
            }
        }

    }
}

