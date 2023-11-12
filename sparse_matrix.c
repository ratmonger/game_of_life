#include <stdlib.h>

struct COOBooleanMatrix {
    unsigned* rows;
    unsigned* cols;
    unsigned num_nonzero;
    unsigned capacity;
}

int find_nonzero(COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    for (int i; i < mtx->num_nonzero; ++i) {
        if ((mtx->rows)[i] == row && (mtx->cols)[i] == col) {
            return i;
        }
    }

    return -1;
}

void copy_array(unsigned* a, unsigned* b, unsigned n) {
    for (int i = 0; i < n; ++i) {
        b[i] = a[i];
    }
}


void double_size(COOBooleanMatrix* mtx) {
    rows_doubled = malloc(2*capacity*sizeof(unsigned));
    cols_doubled = malloc(2*capacity*sizeof(unsigned));

    copy_array(mtx->rows, rows_doubled, mtx->num_nonzero);
    copy_array(mtx->cols, cols_doubled, mtx->num_nonzero);

    free(mtx->rows);
    free(mtx->cols);

    mtx->rows = rows_doubled;
    mtx->cols = cols_doubled;
    mtx->capacity *= 2;
}


void add_nonzero(COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    unsigned nnz = mtx->num_nonzero;

    if (find_nonzero(mtx, row, col) > -1) {
        return;
    }

    if (nnz == capacity) {
        double_size(mtx);
    }

    (mtx->rows)[nnz] = row;
    (mtx->cols)[nnz] = col;

    ++(mtx->num_nonzero);
}


void remove_nonzero(COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    int idx = find_nonzero(mtx, row, col);

    if (idx < 0) {
        return;
    }

    copy(mtx->rows + i*sizeof(unsigned), mtx->rows + (i - 1)*sizeof(unsigned));
    copy(mtx->cols + i*sizeof(unsigned), mtx->cols + (i - 1)*sizeof(unsigned));

    --(mtx->num_nonzero);
}
