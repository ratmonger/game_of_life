#include "mpi_life.hpp"
#include <vector>

using namespace std;

// time calculation speed
void mpi_naive_calc(char* A, char* B, unsigned long dim, int sq_num_procs, int rank_row, int rank_col, int ticks, int rank)
{
    int i,j,k;

    unsigned long pad_dim = dim + 2;

    int N,S,E,W,NW,NE,SW,SE;

    char *temp;

    unsigned long size = dim*dim;
    MPI_Status status;
    
    int num_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    double t0, tfinal;

    N = get_proc((rank_row-1+sq_num_procs) % sq_num_procs, rank_col, sq_num_procs);
    S = get_proc((rank_row+1) % sq_num_procs, rank_col, sq_num_procs);
    E = get_proc(rank_row, (rank_col+1) % sq_num_procs, sq_num_procs);
    W = get_proc(rank_row, (rank_col-1+sq_num_procs) % sq_num_procs, sq_num_procs);

    NW = get_proc((rank_row-1+sq_num_procs) % sq_num_procs, (rank_col-1+sq_num_procs) % sq_num_procs, sq_num_procs);
    NE = get_proc((rank_row-1+sq_num_procs) % sq_num_procs, (rank_col+1) % sq_num_procs, sq_num_procs);
    SW = get_proc((rank_row+1) % sq_num_procs, (rank_col-1+sq_num_procs) % sq_num_procs, sq_num_procs);
    SE = get_proc((rank_row+1) % sq_num_procs, (rank_col+1) % sq_num_procs, sq_num_procs);

    // Allocate arrays of MPI_Request (one per send, one per recv) for all 8 directions
    vector<MPI_Request> send_req(8);
    vector<MPI_Request> recv_req(8);


    /// implement column communication
    //   dim items separated by a stride of pad_dim
    MPI_Datatype column_type;
    MPI_Type_vector(dim, 1, pad_dim, MPI_CHAR, &column_type);
    MPI_Type_commit(&column_type);

    t0 = MPI_Wtime();
    for (int i = 0; i < ticks; i++)
    {
        update(dim, A, B);
        swap(&A, &B);
    }

    tfinal = (MPI_Wtime() - t0) / ticks;
    MPI_Reduce(&tfinal, &t0, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) printf("MPI naive computation cost -- Matrix dim: %lu, Procs: %d, Elapsed Time %e\n", dim * sq_num_procs, num_procs, tfinal);

    MPI_Type_free(&column_type);

}
