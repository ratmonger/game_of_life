#include <stdlib.h>

struct SrcVectors{
    unsigned char *interior;
    unsigned char *top;
    unsigned char *bottom;
    unsigned char *right;
    unsigned char *left;
    unsigned char topR;
    unsigned char topL;
    unsigned char botL;
    unsigned char botR;
};


void init_partition(struct SrcVectors* part, unsigned n) {
    part->interior = malloc(n*n*sizeof(char));
    part->top = malloc(n*sizeof(char));
    part->bottom = malloc(n*sizeof(char));
    part->right = malloc(n*sizeof(char));
    part->left = malloc(n*sizeof(char));
}


void copy_interior(unsigned char* A, unsigned char* B, unsigned n) {
/* Assumes A has (n + 2)^2 elements, B has n^2 elements */
    unsigned A_row, A_col, A_idx;

    for (unsigned i = 0; i < n*n; ++i) {
        A_row = i / n + 1;
        A_col = i % n + 1;
        A_idx = A_row * (n + 2) + A_col;

        B[i] = A[A_idx];
    }
}


void copy_row_interior(unsigned char* A, unsigned char* b, unsigned n, unsigned row) {
    for(unsigned i = 0; i < n; ++i) {
        b[i] = A[row*(n + 2) + i + 1];
    }
}


void copy_col_interior(unsigned char* A, unsigned char* b, unsigned n, unsigned col) {
    for(unsigned i = 0; i < n; ++i) {
        b[i] = A[(n + 2)*(i + 1) + col];
    }
}


struct SrcVectors* partitions(unsigned char* grid, unsigned long n){
    struct SrcVectors* srcVec = malloc(sizeof(struct SrcVectors));
    init_partition(srcVec, n);

    copy_interior(grid, srcVec->interior, n);

    copy_row_interior(grid, srcVec->top, n, 0);
    copy_row_interior(grid, srcVec->bottom, n, n + 1);
    copy_col_interior(grid, srcVec->left, n, 0);
    copy_col_interior(grid, srcVec->right, n, n + 1);
    
    srcVec->topL = grid[0];
    srcVec->topR = grid[n + 1];
    srcVec->botL = grid[(n + 2) * (n + 1)];
    srcVec->botR = grid[(n + 2)*(n + 2) - 1];

    return srcVec;
}
