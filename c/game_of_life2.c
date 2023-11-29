#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>   
#include <mpi.h>

// this file has multiple implementations
// 1. naive
// 2. very dense matrix mult using the coefficient matrix corresponding to the stencil
//     this is very inefficient and consumes memory because its not sparse encoded
// 3. a custom built hierarchy of pointers to neighbor indices
// see the main() function for how to toggle these on/off


char *grid;
char *copy;
char *C;
unsigned long rows,cols;


struct SrcVectors{
  char *partition;
  char *top;
  char *bottom;
  char *right;
  char *left;
  char topR;
  char topL;
  char botL;
  char botR;
};
struct RecvVectors{
  char *top;
  char *bottom;
  char *right;
  char *left;
  char topR;
  char topL;
  char botL;
  char botR;
};



//generate the coefficient matrix for the input dimensions
// this does not generate a sparse matrix and thus should be avoided for large sizes
// until FIXED!!!
void generate_C(){

  unsigned long i,j,n,row_offset;
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


// fills the grid with random 0 and 1
// TODO implement probability here
// creates a grid with border padding cells to avoid out-of-bounds error
void init_grid(){
  unsigned long i,j;
  grid = malloc((rows+2) * (cols+2) * sizeof(char));
  srand(2);//set seed
  for (i = 1; i < rows+1; i++){
    for (j = 1; j< cols+1; j++){
      grid[i*(cols+2) + j] = rand() & 0x1;
    }
  }
}


void print_grid(){
  unsigned long i,j;
  printf("This is the full grid\n");
  printf("\n");
  for (i = 1; i < rows+1; i++){
    for (j = 1; j < cols+1; j++){
      printf("%c", 48+grid[i*(cols+2) + j]);
    }
    printf("\n");
  }
}


// run the game
// perform dense multiplication using C matrix
void run_Dense_Mult(int ticks){

  int sum;
  int count=ticks;
  unsigned long i,j,N;
  char *temp;
  unsigned int liveNeighbors;
  char cellState;
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

    print_grid();

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

void print_sub(char *sub, int n, int rank){
  unsigned long i,j;
  printf("Sub_Grid of rank %d:\n", rank);
  for (i =0; i < n+2; i++){
    for (j = 0; j < n+2; j++){
      printf("%c", 48+sub[i*(n+2) + j]);
    }
    printf("\n");
  }
}
// run the game for number of ticks  using naive method
void run_naive(int ticks, unsigned long r_start, unsigned long r_end,  unsigned long c_start, unsigned long c_end, unsigned long k){
  unsigned long i,j,n;
  int forever = 1;
  unsigned int liveNeighbors; // neighbors of a given cell
  char cellState; // state of the cell: alive or dead
  unsigned long WIDTH = (cols+2);
  unsigned long dim = k*k;
  char *temp;
  int *loop;
  
  printf("rows %ld, col %ld\n", rows, cols);
  // allocate the array to copy the new updated grid
  copy = malloc(dim * sizeof(char));

  while (*loop){
    print_grid();

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
struct SrcVectors partitions(int ticks, int rank, unsigned long num_procs, unsigned long r_rows, unsigned long r_cols, unsigned long n){

  struct SrcVectors srcVec;
    
  srcVec.partition = calloc(n, sizeof(char));//has an extra dimension built in but is 0 at first
  unsigned long r_end = r_rows+n-2;
  unsigned long c_end = r_cols+n-2;
  unsigned long k = 1;//leaves buffer cells 0
  unsigned long q = 1;
  unsigned long i, j, t, p, a=0, b=0, c=0, d=0;
 

  srcVec.top = calloc(n, sizeof(char));//init to all zeros
  srcVec.bottom = calloc(n, sizeof(char));
  srcVec.right = calloc(n, sizeof(char));
  srcVec.left = calloc(n, sizeof(char));
  
  //puts data from grid into partition grid
  //puts edge vectors into separate arrays

  for (i = r_rows; i < r_end; i++){
    q = 1;
    for (j = r_cols; j< c_end; j++){ 
      t =k*(n)+q;
      p = i*(rows+2)+j;
      srcVec.partition[t]= grid[p];

      if(t== (n+q)){
	srcVec.top[a]=grid[p];
	a++;
      }if(t==(n-2)*n+q){
	srcVec.bottom[b]= grid[p];
	b++;
      }if(t==(k*n+1)){
	srcVec.left[c]= grid[p];
	c++;
      }if(t==(k*n+(n-2))){
	srcVec.right[d]= grid[p];
	d++;
      }
      //Diags
      if(t== n+1){
	srcVec.topL=grid[p];
      }if(t==((n-2)*n+1)){
	srcVec.botL= grid[p];
      }if(t==n+(n-2)){
	srcVec.topR= grid[p];
      }if(t==((n-2)*n+(n-2))){
	srcVec.botR= grid[p];
      }
      q++;
    }
    printf("\n");
    k++;
  }
   
  return srcVec;
}


struct SrcVectors get_Vectors(int rank, unsigned long k, struct SrcVectors srcV, unsigned long num_procs){
  struct RecvVectors recvV;
  unsigned long i, j, n;
  int tag = 1234;

  //diagonal tally
  int topR = 0;
  int topL = 0;
  int botL = 0;
  int botR = 0;

  recvV.bottom = calloc(k, sizeof(char));//start as all zeros
  recvV.top = calloc(k ,sizeof(char));
  recvV.left = calloc(k, sizeof(char));
  recvV.right = calloc(k, sizeof(char));
  recvV.botR = 0;
  recvV.topR = 0;
  recvV.botL = 0;
  recvV.topL = 0;


  int sqrt_procs = sqrt(num_procs);
  //communication of edge vectors
  if( rank%sqrt_procs ==0 && rank!= sqrt_procs+1 ){//if has process to the right 
    topR++;
    botR++;
    MPI_Sendrecv(srcV.right, k, MPI_CHAR, rank+1, tag, recvV.right, k,MPI_CHAR, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }if(rank!=0 && rank!= num_procs+1 && rank%sqrt_procs !=0){// if process to the left1`
    topL++;
    botL++;
    MPI_Sendrecv(srcV.left, k, MPI_CHAR, rank-1, tag, recvV.left, k,MPI_CHAR, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }if(rank-sqrt_procs >=0){//if process above
    topR++;
    topL++; 
    MPI_Sendrecv(srcV.top, k, MPI_CHAR, rank-sqrt_procs, tag, recvV.top, k,MPI_CHAR, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }if(rank+sqrt_procs< num_procs){//if process below 
    botL++;
    botR++;
    MPI_Sendrecv(srcV.bottom, k, MPI_CHAR, rank+sqrt_procs, tag, recvV.bottom, k,MPI_CHAR, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  if(topL == 2){
    MPI_Sendrecv(&srcV.topL, k, MPI_CHAR, rank-sqrt_procs-1, tag, &recvV.topL, k,MPI_CHAR, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  if(topR == 2){
    MPI_Sendrecv(&srcV.topR, k, MPI_CHAR, rank-sqrt_procs+1, tag, &recvV.topR, k,MPI_CHAR, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  if(botL == 2){
    MPI_Sendrecv(&srcV.botL, k, MPI_CHAR, rank+sqrt_procs-1, tag, &recvV.botL, k,MPI_CHAR, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  if(botR == 2){
    MPI_Sendrecv(&srcV.botR, k, MPI_CHAR, rank+sqrt_procs+1, tag, &recvV.botR, k,MPI_CHAR, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  int width = k+2;
       
  //adds vectors to grid as buffer row
  int l=0, r= 0, b= 0, t = 0;
  for( i = 0; i<k+2; i++){
    for( j = 0; j<k+2; j++){

      n = (i * (k+2)) + j;
            
      if(n==j && n!=0 && j!= width-1){//first row 
	srcV.partition[n]= recvV.top[t];
	t++;
      }if(n==((width-1)*width+j) && j!=0 && j!=width-1 ){//bottom row
	srcV.partition[n]= recvV.bottom[b];
	b++;
      }if(n==(i*(width)+(width-1))&& i!=0 && i!=width-1){//right column
	srcV.partition[n]= recvV.right[r];
	r++;
      }if(n==(i*(width)+0)&& i!=0 && i!=width-1){//left column
	srcV.partition[n]= recvV.left[l];
	l++;
      }
      //diags
      if(n == width-1){//top right
	srcV.partition[n]=recvV.topR;
      }if(n == 0){//top left
	srcV.partition[n]=recvV.topL;
      }if(n == (width-1)*(width)){//bottom left
	srcV.partition[n]=recvV.botL;
      }if(n == (width-1)*(width) +(width-1)){//bottom right
	srcV.partition[n]=recvV.botR;
      }
    }
  }
  free(recvV.top);
  free(recvV.bottom);
  free(recvV.right);
  free(recvV.left);
  return srcV;
}


void parallel_naive(int ticks, int rank, unsigned long num_procs, unsigned long r_rows, unsigned long r_cols, unsigned long k, struct SrcVectors p){
    
  int count=ticks;//total ticks left to loop
  int *loop;//a toggle variable: loop for ticks or forever
  unsigned long i,j,n;
  int forever = 1;
  unsigned int liveNeighbors; // neighbors of a given cell
  char cellState; // state of the cell: alive or dead
  unsigned long WIDTH = k+2;
  unsigned long dim = k*k;
  char *temp;
  struct RecvVectors recvV;
  copy = malloc(dim * sizeof(char));

  //get the vectors from other processes here, will be all zeros if no processes to communicate with
  MPI_Barrier(MPI_COMM_WORLD);
  p = get_Vectors(rank, k, p, num_procs);

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
        
    //if(rank == 0){
      print_sub(p.partition,k, rank);//prints each subgrid
    //}
        
    forever = 0;

    for (i = 1; i < k; i++){
      for (j = 1; j< k; j++){
                
	n = i * (k+2) + j;
	liveNeighbors = p.partition[n - 1] + //left
	  p.partition[n + 1] + //right  
	  p.partition[n - (WIDTH)] +// top
	  p.partition[n + (WIDTH)] +//bottom
	  p.partition[n - (WIDTH) - 1]+//diag upleft
	  p.partition[n + (WIDTH) - 1]+//diag downleft
	  p.partition[n - (WIDTH) + 1]+//diag upright
	  p.partition[n + (WIDTH) + 1];// diag downright
            

	cellState = p.partition[n];
	forever += cellState;
	//if(rank == 0){
	  printf("cellstate %d, forever %d\n", cellState, forever);
	  // alive
	  printf("liveNeighbors %d\n", liveNeighbors);
	//}
                
	if (p.partition[n]) { 
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
    MPI_Barrier(MPI_COMM_WORLD);
    //MPI_Gather(copy, dim, MPI_CHAR, &grid[r_rows+r_cols], dim, MPI_CHAR, 0, MPI_COMM_WORLD);//should we gather to one process?
    //MPI_Bcast(&grid, (rows + 2) * WIDTH, MPI_CHAR, 0, MPI_COMM_WORLD);
    count--;//decrement ticks

    temp = p.partition;//swap the grids (new and old)
    p.partition = copy;
    copy = temp;
  }

  free(p.top);
  free(p.bottom);
  free(p.right);
  free(p.left);
}



int main( int argc, char *argv[] )  {

  int row_temp,col_temp, ticks;
  float start, end;
  struct SrcVectors part;
  unsigned long r_row; 
  unsigned long r_col; 
  unsigned long n; 
  MPI_Init(&argc, &argv);

  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  if( argc == 3 ) {
    rows = atoi(argv[1]);
    cols = atoi(argv[1]);
    ticks = atoi(argv[2]);
        
    int sqrt_procs = sqrt(num_procs);
    if (sqrt_procs*sqrt_procs != num_procs){
      if (rank == 0) 
	printf("Number of processes needs to be a square\n");
      MPI_Finalize();
      return 1;
    }
    n = rows/sqrt_procs;
        
    r_row = (rank/sqrt_procs)*n +1;
    r_col = (rank%sqrt_procs)*n +1;

    if (n*n*num_procs != rows*rows)
      {
        if (rank == 0) 
	  printf("Cannot evenly split %ld rows and cols over %ld processes\n", rows, num_procs);
        MPI_Finalize();
        return 1;
      }

    if (rows > 0 &&  cols > 0){
        
      init_grid();

      // run the naive implementation
      //run_naive(ticks);
            
      MPI_Barrier(MPI_COMM_WORLD);
      start = MPI_Wtime();   

      part  = partitions(ticks, rank, num_procs, r_row, r_col, n + 2);//sets up partition
      parallel_naive(ticks, rank, num_procs, r_row, r_col, n, part);//runs naive method

      end = MPI_Wtime() - start;
      MPI_Reduce(&end, &start, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);//reduces to get the longest process time.
            
      if (rank == 0){
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

