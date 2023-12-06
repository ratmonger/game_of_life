#include "mpi_life.hpp"
#include <vector>

using namespace std;



void concurrency_naive_testall(char* A, char* B, unsigned long dim, int sq_num_procs, int rank_row, int rank_col, int ticks, int rank)
{
    int i,j,k;

    unsigned long pad_dim = dim + 2;

    int N,S,E,W,NW,NE,SW,SE;

    char *temp;

    unsigned long size = dim*dim;
    MPI_Status status;

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

    /*if (rank == 0){
        print_grid(dim, A);
    }*/

    int ready;

    for (int i = 0; i < ticks; i++)
    {

        MPI_Isend(&(A[pad_dim + 1]), dim, MPI_CHAR, N, 1111, MPI_COMM_WORLD, &(send_req[0]));
        MPI_Irecv(&(A[(dim+1)*(pad_dim) +1]), dim, MPI_CHAR, S, 1111, MPI_COMM_WORLD, &(recv_req[0]));

        MPI_Isend(&(A[(dim*pad_dim)+1]), dim, MPI_CHAR, S, 2222, MPI_COMM_WORLD, &(send_req[1]));
        MPI_Irecv(&(A[1]),     dim, MPI_CHAR, N, 2222, MPI_COMM_WORLD, &(recv_req[1]));

        MPI_Isend(&(A[pad_dim + 1]), 1, column_type, W, 3333, MPI_COMM_WORLD, &(send_req[2]));
        MPI_Irecv(&(A[(2*pad_dim) -1]), 1, column_type, E, 3333, MPI_COMM_WORLD, &(recv_req[2]));

        MPI_Isend(&(A[(2*pad_dim) -2]), 1, column_type, E, 4444, MPI_COMM_WORLD, &(send_req[3]));
        MPI_Irecv(&(A[pad_dim]), 1, column_type, W, 4444, MPI_COMM_WORLD, &(recv_req[3]));

        MPI_Isend(&(A[pad_dim + 1]), 1, MPI_CHAR, NW, 5555, MPI_COMM_WORLD, &(send_req[4]));
        MPI_Irecv(&(A[(pad_dim*pad_dim) - 1]), 1, MPI_CHAR, SE, 5555, MPI_COMM_WORLD, &(recv_req[4]));

        MPI_Isend(&(A[(2*pad_dim) -2]), 1, MPI_CHAR, NE, 6666, MPI_COMM_WORLD, &(send_req[5]));
        MPI_Irecv(&(A[(dim+1)*pad_dim]), 1, MPI_CHAR, SW, 6666, MPI_COMM_WORLD, &(recv_req[5]));

        MPI_Isend(&(A[(dim*pad_dim)+1]), 1, MPI_CHAR, SW, 7777, MPI_COMM_WORLD, &(send_req[6]));
        MPI_Irecv(&(A[pad_dim - 1]), 1, MPI_CHAR, NE, 7777, MPI_COMM_WORLD, &(recv_req[6]));

        MPI_Isend(&(A[((dim+1)*pad_dim) - 2]), 1, MPI_CHAR, SE, 8888, MPI_COMM_WORLD, &(send_req[7]));
        MPI_Irecv(&(A[0]), 1, MPI_CHAR, NW, 8888, MPI_COMM_WORLD, &(recv_req[7]));

        MPI_Testall(8, send_req.data(), &ready, MPI_STATUSES_IGNORE);
        MPI_Testall(8, recv_req.data(), &ready, MPI_STATUSES_IGNORE);

        updateInner(dim, A, B);

        MPI_Waitall(8, send_req.data(), MPI_STATUSES_IGNORE);
        MPI_Waitall(8, recv_req.data(), MPI_STATUSES_IGNORE);

        updateOuter(dim, A, B);
        swap(&A, &B);

        /*if (rank == 0){
            print_grid(dim, A);
        }*/

    }

    MPI_Type_free(&column_type);

}
