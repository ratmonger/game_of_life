#include <stdlib.h>

typedef struct COOBooleanMatrix {
    unsigned* rows;
    unsigned* cols;
    unsigned num_nonzero;
    unsigned capacity;
}

typedef struct SparseBooleanVector {
    unsigned* indices;
    unsigned num_nonzero;
    unsigned capacity;
}

typedef struct SparseCharVector {
    unsigned* indices;
    char* values;
    unsigned num_nonzero;
    unsigned capacity;
}

struct COOBooleanMatrix* init_COO_boolmat(unsigned init_capacity) {
    COOBooleanMatrix* mtx = malloc(sizeof(COOBooleanMatrix));
    mtx->rows = malloc(init_capacity*sizeof(unsigned));
    mtx->cols = malloc(init_capacity*sizeof(unsigned));
    mtx->num_nonzero = 0;
    mtx->capacity = init_capacity;

    return mtx;
}

struct SparseBooleanVector* init_boolvec(unsigned init_capacity) {
    SparseBooleanVector* vec = malloc(sizeof(SparseBooleanVector));
    vec->indices = malloc(init_capacity*sizeof(unsigned));
    vec->num_nonzero = 0;
    vec->capacity = init_capacity;

    return vec;
}

struct SparseCharVector* init_charvec(unsigned init_capacity) {
    SparseBooleanVector* vec = malloc(sizeof(SparseCharVector));
    vec->indices = malloc(init_capacity*sizeof(unsigned));
    vec->values = malloc(init_capacity*sizeof(char));
    vec->num_nonzero = 0;
    vec->capacity = init_capacity;

    return vec;
}

int find_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    for (int i; i < mtx->num_nonzero; ++i) {
        if ((mtx->rows)[i] == row && (mtx->cols)[i] == col) {
            return i;
        }
    }

    return -1;
}

int find_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    for (int i; i < vec->num_nonzero; ++i) {
        if ((vec->indices)[i] == idx) {
            return i;
        }
    }

    return -1;
}

int find_nonzero_charvec(struct SparseCharVector* vec, unsigned idx) {
    for (int i; i < vec->num_nonzero; ++i) {
        if ((vec->indices)[i] == idx) {
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

void double_size_boolmat(struct COOBooleanMatrix* mtx) {
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

void double_size_boolvec(struct SparseBooleanVector* vec) {
    indices_doubled = malloc(2*capacity*sizeof(unsigned));

    copy_array(vec->indices, indices_doubled, vec->num_nonzero);

    free(vec->indices);

    vec->indices = indices_doubled;

    vec->capacity *= 2;
}

void double_size_charvec(struct SparseCharVector* vec) {
    indices_doubled = malloc(2*capacity*sizeof(unsigned));
    values_doubled = malloc(2*capacity*sizeof(char));

    copy_array(vec->indices, indices_doubled, vec->num_nonzero);
    copy_array(vec->values, values_doubled, vec->num_nonzero);

    free(vec->indices);
    free(vec->values);

    vec->indices = indices_doubled;
    vec->values = values_doubled;

    vec->capacity *= 2;
}

void add_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
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

void add_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    unsigned nnz = vec->num_nonzero;

    if (find_nonzero(vec, idx) > -1) {
        return;
    }

    if (nnz == capacity) {
        double_size(vec);
    }

    (vec->indices)[nnz] = idx;

    ++(vec->num_nonzero);
}

void add_nonzero_charvec(struct SparseCharVector* vec, unsigned idx, char val) {
    unsigned nnz = vec->num_nonzero;

    int i = find_nonzero(vec, idx);

    if (i > -1) {
        (vec->values)[i] = val;
        return
    }

    if (nnz == capacity) {
        double_size(vec);
    }

    (vec->indices)[nnz] = idx;
    (vec->values)[nnz] = val;

    ++(vec->num_nonzero);
}


void remove_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    int idx = find_nonzero(mtx, row, col);

    if (idx < 0) {
        return;
    }

    copy(mtx->rows + i*sizeof(unsigned), mtx->rows + (i - 1)*sizeof(unsigned));
    copy(mtx->cols + i*sizeof(unsigned), mtx->cols + (i - 1)*sizeof(unsigned));

    --(mtx->num_nonzero);
}

void remove_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    int i = find_nonzero(vec, idx);

    if (i < 0) {
        return;
    }

    copy(vec->indices + i*sizeof(unsigned), vec->indices + (i - 1)*sizeof(unsigned));

    --(vec->num_nonzero);
}

void remove_nonzero_charvec(struct SparseCharVector* vec, unsigned idx) {
    int i = find_nonzero(vec, idx);

    if (i < 0) {
        return;
    }

    copy(vec->indices + i*sizeof(unsigned), vec->indices + (i - 1)*sizeof(unsigned));

    --(vec->num_nonzero);
}
