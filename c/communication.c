#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include "parallel.h"

#define TRUE 1
#define FALSE 0


const unsigned char VERBOSE_COMM = FALSE;


void communicate_left(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs) {
    unsigned proc_rows = (unsigned) sqrt(num_procs);

    if (rank % proc_rows == 0)
        return;

    MPI_Request send_rqst, recv_rqst;
    int tag = 1234;

    if (VERBOSE_COMM)
        printf("Sending and receiving from proc %d to proc %d...\n", rank, rank-1);

    MPI_Isend(edges->left, grid_width, MPI_CHAR, rank - 1, tag, MPI_COMM_WORLD, &send_rqst);
    MPI_Irecv(grid->left, grid_width, MPI_CHAR, rank - 1, tag, MPI_COMM_WORLD, &recv_rqst);

    MPI_Wait(&send_rqst, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_rqst, MPI_STATUS_IGNORE);
}


void communicate_right(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs) {
    unsigned proc_rows = (unsigned) sqrt(num_procs);

    if (rank % proc_rows == proc_rows - 1)
        return;

    MPI_Request send_rqst, recv_rqst;
    int tag = 1234;

    if (VERBOSE_COMM)
        printf("Sending and receiving from proc %d to proc %d...\n", rank, rank+1);

    MPI_Isend(edges->right, grid_width, MPI_CHAR, rank + 1, tag, MPI_COMM_WORLD, &send_rqst);
    MPI_Irecv(grid->right, grid_width, MPI_CHAR, rank + 1, tag, MPI_COMM_WORLD, &recv_rqst);

    MPI_Wait(&send_rqst, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_rqst, MPI_STATUS_IGNORE);
}


void communicate_above(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs) {
    unsigned proc_rows = (unsigned) sqrt(num_procs);

    if (rank / proc_rows == 0)
        return;

    MPI_Request send_rqst, recv_rqst;
    int tag = 1234;

    if (VERBOSE_COMM)
        printf("Sending and receiving from proc %d to proc %d...\n", rank, rank-proc_rows);
    MPI_Isend(edges->top, grid_width - 2, MPI_CHAR, rank - proc_rows, tag, MPI_COMM_WORLD, &send_rqst);
    MPI_Irecv(grid->top, grid_width - 2, MPI_CHAR, rank - proc_rows, tag, MPI_COMM_WORLD, &recv_rqst);

    MPI_Wait(&send_rqst, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_rqst, MPI_STATUS_IGNORE);
}


void communicate_below(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs) {
    unsigned proc_rows = (unsigned) sqrt(num_procs);

    if (rank / proc_rows == proc_rows - 1)
        return;
    
    MPI_Request send_rqst, recv_rqst;
    int tag = 1234;

    if (VERBOSE_COMM)
        printf("Sending and receiving from proc %d to proc %d...\n", rank, rank+proc_rows);
    MPI_Isend(edges->bottom, grid_width - 2, MPI_CHAR, rank + proc_rows, tag, MPI_COMM_WORLD, &send_rqst);
    MPI_Irecv(grid->bottom, grid_width - 2, MPI_CHAR, rank + proc_rows, tag, MPI_COMM_WORLD, &recv_rqst);

    MPI_Wait(&send_rqst, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_rqst, MPI_STATUS_IGNORE);
}


void communicate_upper_left(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs) {
    unsigned proc_rows = (unsigned) sqrt(num_procs);

    if (rank / proc_rows == 0 || rank % proc_rows == 0)
        return;
    
    MPI_Request send_rqst, recv_rqst;
    int tag = 1234;

    if (VERBOSE_COMM)
        printf("Sending and receiving from proc %d to proc %d...\n", rank, rank-proc_rows-1);
    MPI_Isend(&edges->topL, 1, MPI_CHAR, rank - proc_rows - 1, tag, MPI_COMM_WORLD, &send_rqst);
    MPI_Irecv(&grid->topL, 1, MPI_CHAR, rank - proc_rows - 1, tag, MPI_COMM_WORLD, &recv_rqst);

    MPI_Wait(&send_rqst, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_rqst, MPI_STATUS_IGNORE);
}


void communicate_upper_right(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs) {
    unsigned proc_rows = (unsigned) sqrt(num_procs);

    if (rank / proc_rows == 0 || rank % proc_rows == proc_rows - 1)
        return;
    
    MPI_Request send_rqst, recv_rqst;
    int tag = 1234;

    if (VERBOSE_COMM)
        printf("Sending and receiving from proc %d to proc %d...\n", rank, rank-proc_rows+1);
    MPI_Isend(&edges->topR, 1, MPI_CHAR, rank - proc_rows + 1, tag, MPI_COMM_WORLD, &send_rqst);
    MPI_Irecv(&grid->topR, 1, MPI_CHAR, rank - proc_rows + 1, tag, MPI_COMM_WORLD, &recv_rqst);

    MPI_Wait(&send_rqst, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_rqst, MPI_STATUS_IGNORE);
}


void communicate_lower_right(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs) {
    unsigned proc_rows = (unsigned) sqrt(num_procs);

    if (rank / proc_rows == proc_rows - 1 || rank % proc_rows == proc_rows - 1)
        return;
    
    MPI_Request send_rqst, recv_rqst;
    int tag = 1234;

    if (VERBOSE_COMM)
        printf("Sending and receiving from proc %d to proc %d...\n", rank, rank+proc_rows+1);
    MPI_Isend(&edges->botR, 1, MPI_CHAR, rank + proc_rows + 1, tag, MPI_COMM_WORLD, &send_rqst);
    MPI_Irecv(&grid->botR, 1, MPI_CHAR, rank + proc_rows + 1, tag, MPI_COMM_WORLD, &recv_rqst);

    MPI_Wait(&send_rqst, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_rqst, MPI_STATUS_IGNORE);
}


void communicate_lower_left(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs) {
    unsigned proc_rows = (unsigned) sqrt(num_procs);

    if (rank / proc_rows == proc_rows - 1 || rank % proc_rows == 0)
        return;
    
    MPI_Request send_rqst, recv_rqst;
    int tag = 1234;

    if (VERBOSE_COMM)
        printf("Sending and receiving from proc %d to proc %d...\n", rank, rank+proc_rows-1);
    MPI_Isend(&edges->botL, 1, MPI_CHAR, rank + proc_rows - 1, tag, MPI_COMM_WORLD, &send_rqst);
    MPI_Irecv(&grid->botL, 1, MPI_CHAR, rank + proc_rows - 1, tag, MPI_COMM_WORLD, &recv_rqst);

    MPI_Wait(&send_rqst, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_rqst, MPI_STATUS_IGNORE);
}


void communicate_edges(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs) {
    communicate_left(grid, grid_width, edges, rank, num_procs);
    communicate_right(grid, grid_width, edges, rank, num_procs);
    communicate_above(grid, grid_width, edges, rank, num_procs);
    communicate_below(grid, grid_width, edges, rank, num_procs);

    communicate_upper_left(grid, grid_width, edges, rank, num_procs);
    communicate_upper_right(grid, grid_width, edges, rank, num_procs);
    communicate_lower_right(grid, grid_width, edges, rank, num_procs);
    communicate_lower_left(grid, grid_width, edges, rank, num_procs);
}
