#include <stdlib.h>
#include <stdio.h>
#include<mpi.h>

#include "gol_util.h"
#include "parallel.h"
#include "communication.h"

#define TRUE 1
#define FALSE 0

void print_array(unsigned char* arr, unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void fill_array(unsigned char* arr, unsigned n, char val) {
    for (unsigned i = 0; i < n; ++i) {
        arr[i] = val;
    }
}

unsigned array_equal(unsigned char* a, unsigned char* b, unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return FALSE;
        }
    }

    return TRUE;
}

int main(int argc, char** argv) {
    const unsigned GRID_WIDTH = 5;

    int rank, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    struct DomainEdges* edges = init_edges(GRID_WIDTH);

    if (rank == 0) {
        for (unsigned i = 0; i < GRID_WIDTH; ++i) {
            grid->interior[i*GRID_WIDTH + GRID_WIDTH - 1] = 1;
        }

        printf("Interior:\n\n");
        print_grid_dense(grid->interior, GRID_WIDTH);
        printf("\n");
    }

    get_edges(grid->interior, edges, GRID_WIDTH);

    if (rank == 0) {
       printf("Righthand edge:\n\n");
       print_array(edges->right, GRID_WIDTH);
       printf("\n");
    }

    if (rank == 0)
        communicate_right(grid, GRID_WIDTH, edges, rank, num_procs);
    else if (rank == 1) {
        communicate_left(grid, GRID_WIDTH, edges, rank, num_procs);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 1) {
        unsigned char test_res;
        unsigned char* expected = malloc(GRID_WIDTH*sizeof(char));

        fill_array(expected, GRID_WIDTH, 1);
        
        test_res = array_equal(expected, grid->left, GRID_WIDTH);

        printf("Expected: ");
        print_array(expected, GRID_WIDTH);

        printf("Actual: ");
        print_array(grid->left, GRID_WIDTH);
        printf("\n");

        printf("Communication test result: %d\n", test_res);
    }

    MPI_Finalize();
    return 0;
}



