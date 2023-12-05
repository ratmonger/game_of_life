#include "mpi_life.hpp"
#include <vector>

using namespace std;



void mpi_naive(char* A, char* B, unsigned long dim, int sq_num_procs, int rank_row, int rank_col, int ticks)
{
    int rank, num_procs,i,j,k;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Request send_req, recv_req;

    unsigned long pad_dim = dim + 2;

    int N,S,E,W,NW,NE,SW,SE;

    char *temp;

    unsigned long size = dim*dim;
    //int tag_a = 1234;
    //int tag_b = 4321;
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
    //saving this for later
    //MPI_Type_free(&column)

    MPI_Datatype column;
    MPI_Type_vector(dim, 1, pad_dim, MPI_CHAR, &column);
    MPI_Type_commit(&column);



    print_grid(dim, A);


    for (int i = 0; i < ticks; i++)
    {
        //    MPI_Barrier(MPI_COMM_WORLD);
        // 4a. Send A to send_proc_A
        // 4b. Recv new A from recv_proc_A

        // 4c. Send B to send_proc_B
        // 4c. Recv new B from recv_proc_B


        MPI_Isend(&(A[pad_dim + 1]), dim, MPI_CHAR, N, 1111, MPI_COMM_WORLD, &(send_req[0]));
        MPI_Irecv(&(A[1]),     dim, MPI_CHAR, N, 1111, MPI_COMM_WORLD, &(recv_req[0]));

        MPI_Isend(&(A[(dim*pad_dim)+1]), dim, MPI_CHAR, S, 2222, MPI_COMM_WORLD, &(send_req[1]));
        MPI_Irecv(&(A[(dim+1)*(pad_dim) +1]), dim, MPI_CHAR, S, 2222, MPI_COMM_WORLD, &(recv_req[1]));

        ////
        MPI_Isend(&(A[pad_dim + 1]), 1, MPI_CHAR, NW, 3333, MPI_COMM_WORLD, &(send_req[2]));
        MPI_Irecv(&(A[0]]), 1, MPI_CHAR, NW, 3333, MPI_COMM_WORLD, &(recv_req[2]));

        MPI_Isend(&(A[(2*pad_dim) -2]), 1, MPI_CHAR, NE, 3333, MPI_COMM_WORLD, &(send_req[3]));
        MPI_Irecv(&(A[pad_dim - 1]), 1, MPI_CHAR, NE, 3333, MPI_COMM_WORLD, &(recv_req[3]));



        MPI_Isend(&(A[(dim*pad_dim)+1]), 1, MPI_CHAR, SW, 4444, MPI_COMM_WORLD, &(send_req[4]));
        // quick note
        MPI_Irecv(&(A[(dim+1)*pad_dim]), 1, MPI_CHAR, SW, 4444, MPI_COMM_WORLD, &(recv_req[4]));

        MPI_Isend(&(A[((dim+1)*pad_dim) - 2]), 1, MPI_CHAR, SE, 5555, MPI_COMM_WORLD, &(send_req[5]));
        MPI_Irecv(&(A[(pad_dim*pad_dim) - 1]), 1, MPI_CHAR, SE, 5555, MPI_COMM_WORLD, &(recv_req[5]));


        MPI_Isend(&(A[pad_dim + 1]), 1, column, W, 6666, MPI_COMM_WORLD, &(send_req[6]));
        MPI_Irecv(&(A[pad_dim]]), 1, column, W, 6666, MPI_COMM_WORLD, &(recv_req[6]));

        MPI_Isend(&(A[(2*pad_dim) -2]), 1, column, E, 7777, MPI_COMM_WORLD, &(send_req[7]));
        MPI_Irecv(&(A[(2*pad_dim) -1]), 1, column, E, 7777, MPI_COMM_WORLD, &(recv_req[7]));

        // use wait ALL

        // add in call to update A and B

        // swap A and B

        MPI_Wait(&send_req, &status);
        MPI_Wait(&recv_req, &status);

        tmp = send_B;
        send_B = recv_B;
        recv_B = tmp;

        MPI_Isend(send_B, size, MPI_DOUBLE, send_proc_B, 4321,MPI_COMM_WORLD, &send_req);
        MPI_Irecv(tmp, size, MPI_DOUBLE, recv_proc_B, 4321,MPI_COMM_WORLD, &recv_req);

        MPI_Wait(&send_req, &status);
        MPI_Wait(&recv_req, &status);

        // 4e.local matrix  multiplication C += recv_A * recv_B
        matmat(n, recv_A, recv_B, C);
    }

}
