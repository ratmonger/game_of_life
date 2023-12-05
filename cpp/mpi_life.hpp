#ifndef MPI_LIFE_HPP
#define MPI_LIFE_HPP


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

// Return process in process-row 'row' and
// process-column 'col'
int get_proc(int row, int col, int sq_procs);

// Performs local (serial) update
void update(int n, char* A, char* B);

// Performs local (serial) update
void updateInner(int n, char* A, char* B);

// Performs local (serial) update
void updateOuter(int n, char* A, char* B);

// Allocate a char array wth padded bording, dim is an unpadded dimension
// therefore total size of array is (dim+2) * (dim + 2)
char* init_grid(unsigned long dim);

// print the given char* grid, provided the unpadded dimension
void print_grid(unsigned long dim, char *grid);

#endif
