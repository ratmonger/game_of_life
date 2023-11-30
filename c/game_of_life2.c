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
unsigned char *copy;
unsigned char *C;
unsigned long rows,cols;


//generate the coefficient matrix for the input dimensions
// this does not generate a sparse matrix and thus should be avoided for large sizes
// until FIXED!!!
void generate_C(){

  unsigned long i,n,row_offset;
  int on_left_edge;
  int on_right_edge;
  int on_top_edge;
  int on_bottom_edge;

  // number of rows in C
  n = rows*cols;

  // size of C is very large, sparse would be better!
  C = malloc(rows * rows * cols * cols * sizeof(char));

  for (i = 0; i < n; i++){

    on_left_edge = i % cols == 0;
    on_right_edge = i % cols == (cols - 1);
    on_top_edge = i < cols;
    on_bottom_edge = (i / cols == rows - 1);
    row_offset = i*n;

    // straight neighbors
    if (!on_left_edge)
      C[row_offset + i - 1] = 1;

    if (!on_right_edge)
      C[row_offset + i +  1] = 1;

    if (!on_top_edge)
      C[row_offset + i  - cols] = 1;

    if (!on_bottom_edge)
      C[row_offset + i + cols] = 1;

    // diagonal neighbors
    if ((!on_left_edge) && (!on_top_edge))
      C[row_offset +i - cols - 1] = 1;

    if ((!on_left_edge) && (!on_bottom_edge))
      C[row_offset + i + cols - 1] = 1;

    if ((!on_right_edge) && (!on_top_edge))
      C[row_offset + i - cols + 1] = 1;

    if ((!on_right_edge) && (!on_bottom_edge))
      C[row_offset + i + cols + 1] = 1;

  }

  return;
}


// run the game
// perform dense multiplication using C matrix
void run_Dense_Mult(int ticks){

  // int sum;
  int count=ticks;
  unsigned long i,j,N;
  unsigned char *temp;
  unsigned int liveNeighbors;
  unsigned char cellState;
  int *loop;
  int forever = 1;

  N = rows*cols;


  if (ticks > 0){
    loop = &count;
  } else {
    loop = &forever;
  }

  copy = malloc(rows * cols * sizeof(char));
  while (*loop){


    for (i = 0; i < N; i++){
      liveNeighbors = 0;
      for (j = 0; j < N; j++){
	liveNeighbors += C[i*N + j] * grid[j];
      }

      cellState = grid[i];
      forever += cellState;
      // alive
      if (cellState == 1) {
	if (liveNeighbors < 2 || liveNeighbors > 3) {
	  copy[i] = 0; // dead due to underpopulation or overpopulation
	} else {
	  copy[i] = 1; // survive to the next generation
	}
      } else { // dead
	if (liveNeighbors == 3) {
	  copy[i] = 1; // becomes alive due to reproduction
	} else {
	  copy[i] = 0;   // stay dead
	}
      }
    }
    temp = grid;
    grid = copy;
    copy = temp;
    count--;

  }
  free(copy);
}


// run the game for number of ticks  using naive method
void run_naive(int ticks, unsigned long r_start, unsigned long r_end,  unsigned long c_start, unsigned long c_end, unsigned long k){
  unsigned long i,j,n;
  int forever = 1;
  unsigned int liveNeighbors; // neighbors of a given cell
  char cellState; // state of the cell: alive or dead
  unsigned long WIDTH = (cols+2);
  unsigned long dim = k*k;
  unsigned char *temp;
  int *loop = 0;
  
  printf("rows %ld, col %ld\n", rows, cols);
  // allocate the array to copy the new updated grid
  copy = malloc(dim * sizeof(char));

  while (*loop){

    forever = 0;

    for (i = r_start; i < r_end; i++){
      for (j = c_start; j< c_end+1; j++){
                
	n = (i * (rows+2)) + j;
	printf("i %ld, j %ld, n %ld, width %ld, value %d\n", i, j, n, WIDTH, grid[n]);
                liveNeighbors = grid[n - 1] +
                    grid[n + 1] +
		  grid[n - (WIDTH)] +
		  grid[n + (WIDTH)] +
		  grid[n - (WIDTH) - 1]+
		  grid[n + (WIDTH) - 1]+
		  grid[n - (WIDTH) + 1]+
		  grid[n + (WIDTH) + 1];


                cellState = grid[n];
                forever += cellState;
                printf("cellstate %d, forever %d\n", cellState, forever);
                // alive
                printf("liveNeighbors %d\n", liveNeighbors);
                if (grid[n]) { 
		  if (liveNeighbors < 2 || liveNeighbors > 3) {
		    copy[n] = 0; // dead due to underpopulation or overpopulation
		  } else {
		    copy[n] = 1; // survive to the next generation
		  }
                } else { // dead 
		  if (liveNeighbors == 3) {
		    copy[n] = 1; // becomes alive due to reproduction
		  } else {
		    copy[n] = 0;   // stay dead
		  }
                }

      }
    }

    temp = grid;//swap the grids (new and old)
    grid = copy;
    copy = temp;
  }
  free(copy);
}

void communicate_edges(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs){
    communicate_left(grid, grid_width, edges, rank, num_procs);
    communicate_right(grid, grid_width, edges, rank, num_procs);
    communicate_above(grid, grid_width, edges, rank, num_procs);
    communicate_below(grid, grid_width, edges, rank, num_procs);

    communicate_upper_left(grid, grid_width, edges, rank, num_procs);
    communicate_upper_right(grid, grid_width, edges, rank, num_procs);
    communicate_lower_right(grid, grid_width, edges, rank, num_procs);
    communicate_lower_left(grid, grid_width, edges, rank, num_procs);
}

void parallel_naive(int ticks, int rank, unsigned long num_procs, unsigned long rank_rows, unsigned long rank_cols, unsigned long n, struct AugmentedDomain* p){
    
  int count=ticks;//total ticks left to loop
  int *loop = 0;//a toggle variable: loop for ticks or forever
  int forever = 1;

  struct DomainEdges* edges = init_edges(n);

  //get the vectors from other processes here, will be all zeros if no processes to communicate with
  MPI_Barrier(MPI_COMM_WORLD);
  // communicate_edges(rank, n, p, num_procs);

  if (rank == 0) {
      printf("Subgrid (rank %d, tick %d):\n\n", rank, ticks - count);
      print_grid_dense(p->interior, n);
      printf("\n");
  }

  // set loop type: ticks or indefinite loop
  if (ticks > 0){
    loop = &count;
  } else {
    loop = &forever;
  }
  if(rank == 0){
    //print_grid();
  }
    
  while (*loop){
        
    if(rank == 0){
      printf("Subgrid (rank %d, tick %d):\n\n", rank, ticks - count);
      print_grid_dense(p->interior, n);//prints each subgrid
      printf("\n");
    }
        
    
    communicate_edges(p, n, edges, rank, num_procs);
    forever = 0;

    MPI_Barrier(MPI_COMM_WORLD);

    update_state_parallel(p, n);

    MPI_Barrier(MPI_COMM_WORLD);

    //MPI_Gather(copy, dim, MPI_CHAR, &grid[rank_rows+rank_cols], dim, MPI_CHAR, 0, MPI_COMM_WORLD);//should we gather to one process?
    //MPI_Bcast(&grid, (rows + 2) * WIDTH, MPI_CHAR, 0, MPI_COMM_WORLD);
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
          embed_dense(glider, grid, 9, aug_width*aug_width, 1, 1);
      }

      part = partitions(grid, subgrid_width);//sets up.interior

      if (rank == 0) {
          printf("Initial grid:\n\n");
          print_grid_dense(part->interior, subgrid_width);
          printf("\n");
      }

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

