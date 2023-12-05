#include <stdlib.h>
#include <stdio.h>
#include<mpi.h>

#include "gol_util.h"
#include "parallel.h"
#include "communication.h"

#define TRUE 1
#define FALSE 0

#define GRID_WIDTH 5

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


unsigned test_left_comm(unsigned rank, unsigned num_procs) {
    struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    struct DomainEdges* edges = init_edges(GRID_WIDTH);
    unsigned char test_res = FALSE;

    if (rank == 1) {
        for (unsigned i = 0; i < GRID_WIDTH; ++i) {
            grid->interior[i*GRID_WIDTH] = 1;
        }

        get_edges(grid->interior, edges, GRID_WIDTH);
        communicate_left(grid, GRID_WIDTH, edges, rank, num_procs);
    }
    if (rank == 0) {
        communicate_right(grid, GRID_WIDTH, edges, rank, num_procs);

        unsigned char* expected = malloc(GRID_WIDTH*sizeof(char));
        fill_array(expected, GRID_WIDTH, 1);

        test_res = array_equal(grid->right, expected, GRID_WIDTH);
    }

    free_augmented_domain(grid);
    free_domain_edges(edges);

    return test_res;
}


unsigned test_right_comm(unsigned rank, unsigned num_procs) {
    struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    struct DomainEdges* edges = init_edges(GRID_WIDTH);
    unsigned char test_res = FALSE;

    if (rank == 0) {
        for (unsigned i = 0; i < GRID_WIDTH; ++i) {
            grid->interior[i*GRID_WIDTH + (GRID_WIDTH - 1)] = 1;
        }

        get_edges(grid->interior, edges, GRID_WIDTH);
        communicate_right(grid, GRID_WIDTH, edges, rank, num_procs);
    }
    if (rank == 1) {
        communicate_left(grid, GRID_WIDTH, edges, rank, num_procs);

        unsigned char* expected = malloc(GRID_WIDTH*sizeof(char));
        fill_array(expected, GRID_WIDTH, 1);

        test_res = array_equal(grid->left, expected, GRID_WIDTH);

        MPI_Send(&test_res, 1, MPI_CHAR, 0, 1234, MPI_COMM_WORLD);
    }

    if (rank == 0)
        MPI_Recv(&test_res, 1, MPI_CHAR, 1, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    free_augmented_domain(grid);
    free_domain_edges(edges);

    return test_res;
}


unsigned test_above_comm(unsigned rank, unsigned num_procs) {
    struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    struct DomainEdges* edges = init_edges(GRID_WIDTH);
    unsigned char test_res = FALSE;

    if (rank == 2) {
        for (unsigned i = 0; i < GRID_WIDTH; ++i) {
            grid->interior[i] = 1;
        }

        get_edges(grid->interior, edges, GRID_WIDTH);
        communicate_above(grid, GRID_WIDTH, edges, rank, num_procs);
    }
    if (rank == 0) {
        communicate_below(grid, GRID_WIDTH, edges, rank, num_procs);

        unsigned char* expected = malloc(GRID_WIDTH*sizeof(char));
        fill_array(expected, GRID_WIDTH, 1);

        test_res = array_equal(grid->bottom, expected, GRID_WIDTH);
    }

    free_augmented_domain(grid);
    free_domain_edges(edges);

    return test_res;
}


unsigned test_below_comm(unsigned rank, unsigned num_procs) {
    struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    struct DomainEdges* edges = init_edges(GRID_WIDTH);
    unsigned char test_res = FALSE;

    if (rank == 0) {
        for (unsigned i = 0; i < GRID_WIDTH; ++i) {
            grid->interior[GRID_WIDTH * (GRID_WIDTH - 1) + i] = 1;
        }

        get_edges(grid->interior, edges, GRID_WIDTH);
        communicate_below(grid, GRID_WIDTH, edges, rank, num_procs);
    }
    if (rank == 2) {
        communicate_above(grid, GRID_WIDTH, edges, rank, num_procs);

        unsigned char* expected = malloc(GRID_WIDTH*sizeof(char));
        fill_array(expected, GRID_WIDTH, 1);

        test_res = array_equal(grid->top, expected, GRID_WIDTH);

        MPI_Send(&test_res, 1, MPI_CHAR, 0, 1234, MPI_COMM_WORLD);
    }

    if (rank == 0)
        MPI_Recv(&test_res, 1, MPI_CHAR, 2, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    free_augmented_domain(grid);
    free_domain_edges(edges);

    return test_res;
}


unsigned test_upper_left_comm(unsigned rank, unsigned num_procs) {
    struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    struct DomainEdges* edges = init_edges(GRID_WIDTH);
    unsigned char test_res = FALSE;

    if (rank == 3) {
        (grid->interior)[0] = 1;
        get_edges(grid->interior, edges, GRID_WIDTH);
        communicate_upper_left(grid, GRID_WIDTH, edges, rank, num_procs);
    }
    if (rank == 0) {
        communicate_lower_right(grid, GRID_WIDTH, edges, rank, num_procs);

        test_res = ((grid->botR) == 1);
    }

    free_augmented_domain(grid);
    free_domain_edges(edges);

    return test_res;
}


unsigned test_lower_left_comm(unsigned rank, unsigned num_procs) {
    struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    struct DomainEdges* edges = init_edges(GRID_WIDTH);
    unsigned char test_res = FALSE;

    if (rank == 1) {
        (grid->interior)[GRID_WIDTH*(GRID_WIDTH-1)] = 1;
        get_edges(grid->interior, edges, GRID_WIDTH);
        communicate_lower_left(grid, GRID_WIDTH, edges, rank, num_procs);
    }
    if (rank == 2) {
        communicate_upper_right(grid, GRID_WIDTH, edges, rank, num_procs);

        test_res = ((grid->topR) == 1);

        MPI_Send(&test_res, 1, MPI_CHAR, 0, 1234, MPI_COMM_WORLD);
    }

    if (rank == 0)
        MPI_Recv(&test_res, 1, MPI_CHAR, 2, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    free_augmented_domain(grid);
    free_domain_edges(edges);

    return test_res;
}


unsigned test_upper_right_comm(unsigned rank, unsigned num_procs) {
    struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    struct DomainEdges* edges = init_edges(GRID_WIDTH);
    unsigned char test_res = FALSE;

    if (rank == 2) {
        (grid->interior)[GRID_WIDTH-1] = 1;
        get_edges(grid->interior, edges, GRID_WIDTH);
        communicate_upper_right(grid, GRID_WIDTH, edges, rank, num_procs);
    }
    if (rank == 1) {
        communicate_lower_left(grid, GRID_WIDTH, edges, rank, num_procs);

        test_res = ((grid->botL) == 1);

        MPI_Send(&test_res, 1, MPI_CHAR, 0, 1234, MPI_COMM_WORLD);
    }

    if (rank == 0)
        MPI_Recv(&test_res, 1, MPI_CHAR, 1, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    free_augmented_domain(grid);
    free_domain_edges(edges);

    return test_res;
}


unsigned test_lower_right_comm(unsigned rank, unsigned num_procs) {
    struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    struct DomainEdges* edges = init_edges(GRID_WIDTH);
    unsigned char test_res = FALSE;

    if (rank == 0) {
        (grid->interior)[GRID_WIDTH*GRID_WIDTH - 1] = 1;
        get_edges(grid->interior, edges, GRID_WIDTH);
        communicate_lower_right(grid, GRID_WIDTH, edges, rank, num_procs);
    }
    if (rank == 3) {
        communicate_upper_left(grid, GRID_WIDTH, edges, rank, num_procs);

        test_res = ((grid->topL) == 1);

        MPI_Send(&test_res, 1, MPI_CHAR, 0, 1234, MPI_COMM_WORLD);
    }

    if (rank == 0)
        MPI_Recv(&test_res, 1, MPI_CHAR, 3, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    free_augmented_domain(grid);
    free_domain_edges(edges);

    return test_res;
}


unsigned test_comm_edges(unsigned rank, unsigned num_procs) {
    unsigned l_res, r_res, t_res, b_res, ll_res, lr_res, ul_res, ur_res;

    l_res = test_left_comm(rank, num_procs);
    r_res = test_right_comm(rank, num_procs);
    t_res = test_above_comm(rank, num_procs);
    b_res = test_below_comm(rank, num_procs);

    ll_res = test_lower_left_comm(rank, num_procs);
    lr_res = test_lower_right_comm(rank, num_procs);
    ur_res = test_upper_right_comm(rank, num_procs);
    ul_res = test_upper_left_comm(rank, num_procs);

    if (rank == 0) {
        printf("\tLeft: %d\n", l_res);
        printf("\tRight: %d\n", r_res);
        printf("\tAbove: %d\n", t_res);
        printf("\tBelow: %d\n", b_res);

        printf("\tUpper left: %d\n", ul_res);
        printf("\tUpper right: %d\n", ur_res);
        printf("\tLower left: %d\n", ll_res);
        printf("\tLower right: %d\n", lr_res);
    }

    return l_res && r_res && t_res && b_res && ll_res && lr_res && ur_res && ul_res;
}

int main(int argc, char** argv) {

    int rank, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    unsigned comm_res;

    // struct AugmentedDomain* grid = init_domain(GRID_WIDTH);
    // struct DomainEdges* edges = init_edges(GRID_WIDTH);

    if (rank == 0)
        printf("Testing communicate edges...\n");

    comm_res = test_comm_edges(rank, num_procs);

    if (rank == 0)
        printf("Communicate edges: %d\n", comm_res);

    MPI_Finalize();

    return 0;
}



