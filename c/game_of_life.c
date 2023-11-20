#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sparse.h"

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

struct COOBooleanMatrix* generate_COO_stencil_matrix(unsigned n) {
    /* Initializes a stencil matrix for an n x n grid. Note that the
     * dimensions of the matrix are n*n x n*n.
    */
    unsigned rows = n*n;
    unsigned on_left_edge, on_righ_edge, on_top_edge, on_bottom_edge;
    unsigned row, col;

    struct COOBooleanMatrix mtx* = init_COO_boolmat(8*rows);

    for (int i = 0; i < n*n; i++){
        cell_row = i / n;
        cell_col = i % n;

        on_left_edge = (cell_col == 0);
        on_right_edge = (cell_col == n - 1);
        on_top_edge = (cell_row == 0);
        on_bottom_edge = (cell_row == n - 1);

        // straight neighbors
        if (!on_left_edge)
            set_non_zero_boolmat(mtx, i, i-1);

        if (!on_right_edge)
            set_non_zero_boolmat(mtx, i, i+1);

        if (!on_top_edge)
            set_non_zero_boolmat(mtx, i, i - n);

        if (!on_bottom_edge)
            set_non_zero_boolmat(mtx, i, i + n);

        // diagonal neighbors
        if ((!on_left_edge) && (!on_top_edge))
            set_non_zero_boolmat(mtx, i, i - n - 1);

        if ((!on_left_edge) && (!on_bottom_edge))
            set_non_zero_boolmat(mtx, i, i + n - 1);

        if ((!on_right_edge) && (!on_top_edge))
            set_non_zero_boolmat(mtx, i, i - n + 1);

        if ((!on_right_edge) && (!on_bottom_edge))
            set_non_zero_boolmat(mtx, i, i + n + 1);

        return mtx;
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
    printf("\n");
    for (i = 1; i < rows+1; i++){
        for (j = 1; j < cols+1; j++){
            printf("%c", 48+grid[i*(cols+2) + j]);
        }
        printf("\n");
    }
}



// run the game for number of ticks  using naive method
void run_naive(int ticks){

    int count=ticks;//total ticks left to loop
    unsigned long i,j,n;
    char *temp;
    unsigned int liveNeighbors; // neighbors of a given cell
    char cellState; // state of the cell: alive or dead
    int *loop;//a toggle variable: loop for ticks or forever
    int forever = 1;
    unsigned long WIDTH = (cols+2);

    // set loop type: ticks or indefinite loop
    if (ticks > 0){
        loop = &count;
    } else {
        loop = &forever;
    }

    // allocate the array to copy the new updated grid
    copy = malloc((rows+2) * (WIDTH) * sizeof(char));

    while (*loop){
        print_grid();

        forever = 0;

        for (i = 1; i < rows+1; i++){
            for (j = 1; j< cols+1; j++){

                n = (i * (rows+2)) + j;

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
                // alive
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
        count--;//decrement ticks

        temp = grid;//swap the grids (new and old)
        grid = copy;
        copy = temp;
    }
    free(copy);
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

int main( int argc, char *argv[] )  {

    int row_temp,col_temp, ticks;
    if( argc == 4 ) {
        rows = atoi(argv[1]);
        cols = atoi(argv[2]);
        ticks = atoi(argv[3]);

        if (rows > 0 &&  cols > 0){

            init_grid();

            // run the naive implementation
            run_naive(ticks);

            // WARNING DENSE HAS NOT BEEN FITTED FOR THE NEW GRID (WIDTH+2)*(HEIGHT+2) FORMAT
            // remove comments in this block to perform multiplication
            //generate_C();
            //run_Dense_Mult(ticks);
            //free(C);

            free(grid);
        } else {
            printf("Invalid arguments.\nRows and columns must be greater than 0.\n");
        }
    } else {
        printf("Incorrect number of arguments.\nPlease provide: {rows} {columns} {ticks}\n"
                "If (ticks < 1), program will indefinitely loop until all cells are dead.\n");
    }
    return 0;
}

