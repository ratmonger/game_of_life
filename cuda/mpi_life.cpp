#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#include "mpi_life.hpp"

// Returns rank of process in 
//    (process-row 'row', process-column 'col')
int get_proc(int row, int col, int sq_procs)
{
    return row*sq_procs + col;
}


// swaps the pointers of 2 char arrays
void swap(char **A, char **B){
    char *temp = *A;
    *A = *B;
    *B = temp;
}


// mallocs a char array with border padding
// total size = (dim + 2) * (dim + 2)
char* init_grid(unsigned long dim){
    unsigned long i,j;
    char* temp;
    temp = (char *) malloc((dim+2) * (dim+2) * sizeof(char));
    for (i = 0; i < dim+2; i++){
        for (j = 0; j< dim+2; j++){
            temp[i*(dim+2) + j] = 0;
        }
    }
    return temp;
}


// mallocs a char array with border padding
// total size = (dim + 2) * (dim + 2)
char* agar(unsigned long dim){
    unsigned long i,j;
    char* temp;
    temp = (char *) malloc((dim+2) * (dim+2) * sizeof(char));

    for (i = 0; i < dim+2; i++){
        for (j = 0; j< dim+2; j++){
            temp[i*(dim+2) + j] = 0;
        }
    }
    for (i = 0; i < dim+2; i+=2){
        for (j = 0; j< dim+2; j++){
            temp[i*(dim+2) + j] = 1;
        }
    }

    return temp;
}

// fills the grid with random 0 and 1
// creates a grid with border padding cells to avoid out-of-bounds error
char* rand_grid(unsigned long dim){
    unsigned long i,j;
    char* temp;
    srand(2);
    temp = (char *) malloc((dim+2) * (dim+2) * sizeof(char));
    for (i = 0; i < dim+2; i++){
        for (j = 0; j< dim+2; j++){
            temp[i*(dim+2) + j] = rand() & 0x1;
        }
    }
    return temp;
}




// print the given char* grid, provided the unpadded dimension
void print_grid(unsigned long dim, char *grid){
    unsigned long i,j;
    printf("\n");
    for (i = 1; i < dim+1; i++){
        for (j = 1; j < dim+1; j++){
            printf("%c", 48+grid[i*(dim+2) + j]);
        }
        printf("\n");
    }
}


// Performs local (serial) update, A old, B new
// does NOT swap A and B
void update(unsigned long dim, char* A, char* B){

    unsigned long i,j;
    unsigned long n;
    unsigned long padDim = dim + 2;
    int liveNeighbors;

    for (i = 1; i < dim+1; i++){
        for (j = 1; j< dim+1; j++){

            n = (i * (dim+2)) + j;

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

}


// Performs local (serial)INNER grid update, A old, B new
// does NOT swap A and B
void updateInner(unsigned long dim, char* A, char* B){
    unsigned long i,j;
    unsigned long n;
    unsigned long padDim = dim + 2;
    int liveNeighbors;

    for (i = 2; i < dim; i++){
        for (j = 2; j< dim; j++){

            n = (i * (dim+2)) + j;

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

}


// Performs local (serial) OUTER grid update, A old, B new
// does NOT swap A and B
// dim is the actual submatrix size
void updateOuter(unsigned long dim, char* A, char* B){
    unsigned long i,j;
    unsigned long n;
    unsigned long padDim = dim + 2;
    int liveNeighbors;

    for (i = 1; i < dim + 1; i+=(dim -1)){
        for (j = 1; j< dim + 1; j++){

            n = (i * (dim+2)) + j;

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


    for (i = 1; i < dim + 1; i++){
        for (j = 1; j< dim + 1; j+= dim -1){

            n = (i * (dim+2)) + j;

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
}


// Performs local (serial) update, A old, B new
// does NOT swap A and B
void updateThreads(unsigned long dim, char* A, char* B){

    unsigned long i,j;
    unsigned long n;
    unsigned long padDim = dim + 2;
    int liveNeighbors;

#pragma omp parallel for collapse(2)
    for (i = 1; i < dim+1; i++){
        for (j = 1; j< dim+1; j++){

            n = (i * (dim+2)) + j;

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

}

// Performs local (serial)INNER grid update, A old, B new
// does NOT swap A and B
void updateInnerThread(unsigned long dim, char* A, char* B){
    unsigned long i,j;
    unsigned long n;
    unsigned long padDim = dim + 2;
    int liveNeighbors;

#pragma omp parallel for collapse(2)
    for (i = 2; i < dim; i++){
        for (j = 2; j< dim; j++){

            n = (i * (dim+2)) + j;

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

}


// Performs local (serial) OUTER grid update, A old, B new
// does NOT swap A and B
// dim is the actual submatrix size
void updateOuterThread(unsigned long dim, char* A, char* B){
    unsigned long i,j;
    unsigned long n;
    unsigned long padDim = dim + 2;
    int liveNeighbors;

#pragma omp parallel for collapse(2)
    for (i = 1; i < dim + 1; i+=(dim -1)){
        for (j = 1; j< dim + 1; j++){

            n = (i * (dim+2)) + j;

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

#pragma omp parallel for collapse(2)
    for (i = 1; i < dim + 1; i++){
        for (j = 1; j< dim + 1; j+= dim -1){

            n = (i * (dim+2)) + j;

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
}

