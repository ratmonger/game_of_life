#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>   
#include <mpi.h>
#include "gol_util.h"
#include "parallel.h"
#include "communication.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// this file has multiple implementations
// 1. naive
// 2. very dense matrix mult using the coefficient matrix corresponding to the stencil
//     this is very inefficient and consumes memory because its not sparse encoded
// 3. a custom built hierarchy of pointers to neighbor indices
// see the main() function for how to toggle these on/off

unsigned char VERBOSE = TRUE;

unsigned char *grid;

void parallel_naive(int ticks, int rank, unsigned long num_procs, unsigned long rank_rows, unsigned long rank_cols, unsigned long n, struct AugmentedDomain* p){
    
  int count=ticks;//total ticks left to loop
  int *loop = 0;//a toggle variable: loop for ticks or forever
  int forever = 1;

  struct DomainEdges* edges = init_edges(n);

  //get the vectors from other processes here, will be all zeros if no processes to communicate with
  MPI_Barrier(MPI_COMM_WORLD);
  // communicate_edges(rank, n, p, num_procs);

  // set loop type: ticks or indefinite loop
  if (ticks > 0){
    loop = &count;
  } else {
    loop = &forever;
  }
    
  while (*loop){
        
    if(rank < 2){
      printf("Subgrid (rank %d, tick %d):\n\n", rank, ticks - count);
      print_grid_dense(p->interior, n);//prints each subgrid
      printf("\n");
    }
        
    
    if (rank == 0 && !is_empty(edges, n)) {
        printf("Nonzero values to communicate!\n");
    }
    get_edges(p->interior, edges, n);
    communicate_edges(p, n, edges, rank, num_procs);

    if (rank == 1 && count_exterior(p, n) > 0) {
        printf("Rank %d received %d non-empty neighbors (tick %d)!\n", rank, count_exterior(p, n), ticks - count);
    }
    forever = 0;

    MPI_Barrier(MPI_COMM_WORLD);

    update_state_parallel(p, n);

    MPI_Barrier(MPI_COMM_WORLD);

    count--;//decrement ticks
  }

  free_domain_edges(edges);
}



int main( int argc, char *argv[] )  {

  // int row_temp,col_temp;
  int ticks;
  float start, end;
  struct AugmentedDomain* part;
  unsigned long rank_row; 
  unsigned long rank_col; 
  unsigned long grid_width, subgrid_width, aug_width;
  MPI_Init(&argc, &argv);

  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if( argc == 3 ) {
    grid_width = atoi(argv[1]);
    ticks = atoi(argv[2]);
        
    int proc_rows = sqrt(num_procs);

    if (proc_rows*proc_rows != num_procs){
        if (rank == 0) 
	        printf("Number of processes needs to be a square\n");
            MPI_Finalize();
        return 1;
    }

    subgrid_width = grid_width / proc_rows;
        
    rank_row = (rank / proc_rows)*subgrid_width +1;
    rank_col = (rank % proc_rows)*subgrid_width +1;

    if (subgrid_width*subgrid_width*num_procs != grid_width*grid_width)
    {
        if (rank == 0) 
	        printf("Cannot evenly split %ld rows and cols over %d processes\n", grid_width, num_procs);

        MPI_Finalize();

        return 1;
     }

    if (grid_width > 0){
      aug_width = subgrid_width + 2;
        
      // init_grid();
      grid = empty_grid_dense(aug_width);

      if (rank == 0) {
          unsigned char* glider = glider_dense();
          embed_dense(glider, grid, 9, aug_width*aug_width, 1, subgrid_width-3);
      }

      part = partitions(grid, subgrid_width);//sets up.interior

      MPI_Barrier(MPI_COMM_WORLD);
      start = MPI_Wtime();   

      parallel_naive(ticks, rank, num_procs, rank_row, rank_col, subgrid_width, part);//runs naive method


      MPI_Barrier(MPI_COMM_WORLD);
      end = MPI_Wtime() - start;
      MPI_Reduce(&end, &start, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);//reduces to get the longest process time.
            
      if (rank == 0) {
	      printf("Naive Time %e\n", start);
      } 

      // WARNING DENSE HAS NOT BEEN FITTED FOR THE NEW GRID (WIDTH+2)*(HEIGHT+2) FORMAT
      // remove comments in this block to perform multiplication
      //generate_C();
      //run_Dense_Mult(ticks);
      //free(C);
      free(grid);
            
    } else {
      if(rank == 0){
	printf("Invalid arguments.\nRows and columns must be greater than 0.\n");
      }
      MPI_Finalize();
      return 1;
    }
  } else {
    if(rank == 0){
      printf("Incorrect number of arguments.\nPlease provide: {n dimension} {ticks}\n"
	     "If (ticks < 1), program will indefinitely loop until all cells are dead.\n");
    }
    MPI_Finalize();
    return 1;
  }
  MPI_Finalize();
  return 0;
}

