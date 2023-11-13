#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

unsigned long ***neb_list;


// creates a condensed pointer hierarchy to neighbor indices
void generate_Neighbors(){

    unsigned long i,j,n,m, count, neb_list_count,grid_idx;
    int on_left_edge;
    int on_right_edge;
    int on_top_edge;
    int on_bottom_edge;
    int count_neighbors;
    unsigned long **temp;
    unsigned int list_total,stride;

    list_total = (n / 1000000) + 1;
    stride = 1000000;

    // number of rows in C
    n = rows*cols;
    list_total = (n / 1000000) + 1;

    neb_list = malloc( ((n / 1000000) + 1) * sizeof(unsigned long** ));

    count = 0;
    neb_list_count = 0;
    //while (count < n){

    for (i = 0; i < list_total; i++) {
        if (i == list_total - 1){
            if (n >= 1000000){
                stride = n % 1000000;
            } else {
                stride = 1000000 - n;
            }
        }


        neb_list[i] = malloc( 1000000 * sizeof(unsigned long* ));

        for (j = 0; j < stride; j++){

            grid_idx = (i*1000000) + j;
            count_neighbors = 0;
            count = 0;
            on_left_edge = grid_idx % cols == 0;
            on_right_edge = grid_idx % cols == (cols - 1);
            on_top_edge = grid_idx < cols;
            on_bottom_edge = (grid_idx / cols == rows - 1);
            //row_offset = grid_idx*n;

            // straight neighbors
            if (!on_left_edge)
                count_neighbors++;

            if (!on_right_edge)
                count_neighbors++;

            if (!on_top_edge)
                count_neighbors++;

            if (!on_bottom_edge)
                count_neighbors++;

            // diagonal neighbors
            if ((!on_left_edge) && (!on_top_edge))
                count_neighbors++;

            if ((!on_left_edge) && (!on_bottom_edge))
                count_neighbors++;

            if ((!on_right_edge) && (!on_top_edge))
                count_neighbors++;

            if ((!on_right_edge) && (!on_bottom_edge))
                count_neighbors++;



            neb_list[i][j] = malloc(1 + count_neighbors * sizeof(unsigned long* ));


            neb_list[i][j][count++] = count_neighbors;

            // straight neighbors
            if (!on_left_edge)
                neb_list[i][j][count++] =  grid_idx - 1;

            if (!on_right_edge)
                neb_list[i][j][count++] =  grid_idx +  1;

            if (!on_top_edge)
                neb_list[i][j][count++] =  grid_idx  - cols;

            if (!on_bottom_edge)
                neb_list[i][j][count++] = grid_idx + cols;

            // diagonal neighbors
            if ((!on_left_edge) && (!on_top_edge))
                neb_list[i][j][count++] = grid_idx - cols - 1;

            if ((!on_left_edge) && (!on_bottom_edge))
                neb_list[i][j][count++] = grid_idx + cols - 1;

            if ((!on_right_edge) && (!on_top_edge))
                neb_list[i][j][count++] = grid_idx - cols + 1;

            if ((!on_right_edge) && (!on_bottom_edge))
                neb_list[i][j][count++] = grid_idx + cols + 1;
        }
        //neb_list_count++;
    }
}



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
void init_Grid(){
    unsigned long i,j;
    grid = malloc(rows * cols * sizeof(char));
    srand(2);//set seed
    for (i = 0; i < rows; i++){
        for (j = 0; j< cols; j++){
            grid[i*cols + j] = rand() & 0x1;
        }
    }
}


void print_Grid(){
    unsigned long i,j;
    printf("\n");
    for (i = 0; i < rows; i++){
        for (j = 0; j< cols; j++){
            printf("%c", 48+grid[i*cols + j]);
        }
        printf("\n");
    }
}



// run the game for number of ticks  using naive method
void run_Naive(int ticks){

    int count=ticks;
    unsigned long i,j,n,x;
    char *temp;
    int on_left_edge;
    int on_right_edge;
    int on_top_edge;
    int on_bottom_edge;
    unsigned int liveNeighbors;
    char cellState;
    int *loop;
    int forever = 1;

    if (ticks > 0){
        loop = &count;
    } else {
        loop = &forever;
    }

    copy = malloc(rows * cols * sizeof(char));
    while (*loop){
        print_Grid();

        forever = 0;
        //copy = malloc(rows * cols * sizeof(char));
        for (i = 0; i < rows; i++){
            for (j = 0; j< cols; j++){

                n = i*cols + j;
                on_left_edge = n % cols == 0;
                on_right_edge = n % cols == (cols - 1);
                on_top_edge = n < cols;
                on_bottom_edge = (n / cols == rows - 1);
                liveNeighbors = 0;

                // straight neighbors
                if (!on_left_edge)
                    liveNeighbors += grid[n - 1];

                if (!on_right_edge)
                    liveNeighbors += grid[n + 1];

                if (!on_top_edge)
                    liveNeighbors += grid[n - cols];

                if (!on_bottom_edge)
                    liveNeighbors += grid[n + cols];

                // diagonal neighbors
                if ((!on_left_edge) && (!on_top_edge))
                    liveNeighbors += grid[n - cols - 1];

                if ((!on_left_edge) && (!on_bottom_edge))
                    liveNeighbors += grid[n + cols - 1];

                if ((!on_right_edge) && (!on_top_edge))
                    liveNeighbors += grid[n - cols + 1];

                if ((!on_right_edge) && (!on_bottom_edge))
                    liveNeighbors += grid[n + cols + 1];


                cellState = grid[n];
                forever += cellState;
                // alive
                if (cellState == 1) { 
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
        count--;

        temp = grid;
        grid = copy;
        copy = temp;
        //memset?
        //free(temp);
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

        print_Grid();

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


// run the game using the pointer hierarchy of neighbor indices
void run_Neighbor_Array(int ticks){
    int sum;
    int count=ticks;
    unsigned long i,j,k,N,M,neb_list_count, stride, grid_idx;
    char *temp;
    unsigned int liveNeighbors;
    char cellState;
    int *loop;
    int forever = 1;

    N = rows*cols;
    neb_list_count = ((N / 1000000) + 1);
    stride = 1000000;

    if (ticks > 0){
        loop = &count;
    } else {
        loop = &forever;
    }

    //printf("neb list entry  %ld\n", neb_list[i][j][0]);
    copy = malloc(rows * cols * sizeof(char));
    while (*loop){

        print_Grid();

        for (i = 0; i < neb_list_count; i++) {
            if (i == neb_list_count - 1){

                stride = N % 1000000;
            }

            for (j = 0; j < stride; j++){

                grid_idx = (i*1000000) + j;

                liveNeighbors = 0;
                M = neb_list[i][j][0];//get length of array stashed in first index

                for (k = 0; k < M; k++){

                    liveNeighbors += grid[ neb_list[i][j][k+1] ];
                }

                cellState = grid[grid_idx];
                forever += cellState;
                // alive
                if (cellState == 1) {
                    if (liveNeighbors < 2 || liveNeighbors > 3) {
                        copy[grid_idx] = 0; // dead due to underpopulation or overpopulation
                    } else {
                        copy[grid_idx] = 1; // survive to the next generation
                    }
                } else { // dead
                    if (liveNeighbors == 3) {
                        copy[grid_idx] = 1; // becomes alive due to reproduction
                    } else {
                        copy[grid_idx] = 0;   // stay dead
                    }
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
        row_temp = atoi(argv[1]);
        col_temp = atoi(argv[2]);
        ticks = atoi(argv[3]);

        if (row_temp > 0 &&  col_temp > 0){
            rows = row_temp;
            cols = col_temp;

            init_Grid();

            // run the naive implementation
            run_Naive(ticks);

            // remove comments in this block to use pointer neighbor indice structure
            //generate_Neighbors();
            //run_Neighbor_Array(ticks);

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

