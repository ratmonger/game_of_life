#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *grid;
char *copy;
char *C;
unsigned long rows,cols;



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

