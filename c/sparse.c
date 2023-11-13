#include <stdlib.h>

struct COOBooleanMatrix {
    unsigned* rows;
    unsigned* cols;
    unsigned num_nonzero;
    unsigned capacity;
};

struct SparseBooleanVector {
    unsigned* indices;
    unsigned num_nonzero;
    unsigned capacity;
};

struct SparseCharVector {
    unsigned* indices;
    unsigned char* values;
    unsigned num_nonzero;
    unsigned capacity;
};

struct COOBooleanMatrix* init_COO_boolmat(unsigned init_capacity) {
    struct COOBooleanMatrix* mtx = malloc(sizeof(struct COOBooleanMatrix));
    mtx->rows = malloc(init_capacity*sizeof(unsigned));
    mtx->cols = malloc(init_capacity*sizeof(unsigned));
    mtx->num_nonzero = 0;
    mtx->capacity = init_capacity;

    return mtx;
}

struct SparseBooleanVector* init_boolvec(unsigned init_capacity) {
    struct SparseBooleanVector* vec = malloc(sizeof(struct SparseBooleanVector));
    vec->indices = malloc(init_capacity*sizeof(unsigned));
    vec->num_nonzero = 0;
    vec->capacity = init_capacity;

    return vec;
}

struct SparseCharVector* init_charvec(unsigned init_capacity) {
    struct SparseCharVector* vec = malloc(sizeof(struct SparseCharVector));
    vec->indices = malloc(init_capacity*sizeof(unsigned));
    vec->values = malloc(init_capacity*sizeof(char));
    vec->num_nonzero = 0;
    vec->capacity = init_capacity;

    return vec;
}

int find_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    for (int i = 0; i < mtx->num_nonzero; ++i) {
        if ((mtx->rows)[i] == row && (mtx->cols)[i] == col) {
            return i;
        }
    }

    return -1;
}

int find_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    for (int i = 0; i < vec->num_nonzero; ++i) {
        if ((vec->indices)[i] == idx) {
            return i;
        }
    }

    return -1;
}

int find_nonzero_charvec(struct SparseCharVector* vec, unsigned idx) {
    for (int i = 0; i < vec->num_nonzero; ++i) {
        if ((vec->indices)[i] == idx) {
            return i;
        }
    }

    return -1;
}

unsigned char get_value_charvec(struct SparseCharVector* vec, unsigned idx) {
    for (int i = 0; i < vec->num_nonzero; ++i) {
        if ((vec->indices)[i] == idx) {
            return (vec->values)[i];
        }
    }

    return 0;
}

void copy_array_unsigned(unsigned* a, unsigned* b, unsigned n) {
    for (int i = 0; i < n; ++i) {
        b[i] = a[i];
    }
}

void copy_array_char(unsigned char* a, unsigned char* b, unsigned n) {
    for (int i = 0; i < n; ++i) {
        b[i] = a[i];
    }
}

void double_size_boolmat(struct COOBooleanMatrix* mtx) {
    unsigned* rows_doubled = malloc(2*(mtx->capacity)*sizeof(unsigned));
    unsigned* cols_doubled = malloc(2*(mtx->capacity)*sizeof(unsigned));

    copy_array_unsigned(mtx->rows, rows_doubled, mtx->num_nonzero);
    copy_array_unsigned(mtx->cols, cols_doubled, mtx->num_nonzero);

    free(mtx->rows);
    free(mtx->cols);

    mtx->rows = rows_doubled;
    mtx->cols = cols_doubled;
    mtx->capacity *= 2;
}

void double_size_boolvec(struct SparseBooleanVector* vec) {
    unsigned* indices_doubled = malloc(2*(vec->capacity)*sizeof(unsigned));

    copy_array_unsigned(vec->indices, indices_doubled, vec->num_nonzero);

    free(vec->indices);

    vec->indices = indices_doubled;

    vec->capacity *= 2;
}

void double_size_charvec(struct SparseCharVector* vec) {
    unsigned* indices_doubled = malloc(2*(vec->capacity)*sizeof(unsigned));
    unsigned char* values_doubled = malloc(2*(vec->capacity)*sizeof(char));

    copy_array_unsigned(vec->indices, indices_doubled, vec->num_nonzero);
    copy_array_char(vec->values, values_doubled, vec->num_nonzero);

    free(vec->indices);
    free(vec->values);

    vec->indices = indices_doubled;
    vec->values = values_doubled;

    vec->capacity *= 2;
}

void set_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    unsigned nnz = mtx->num_nonzero;

    if (find_nonzero_boolmat(mtx, row, col) > -1) {
        return;
    }

    if (nnz == mtx->capacity) {
        double_size_boolmat(mtx);
    }

    (mtx->rows)[nnz] = row;
    (mtx->cols)[nnz] = col;

    ++(mtx->num_nonzero);
}

void set_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    unsigned nnz = vec->num_nonzero;

    if (find_nonzero_boolvec(vec, idx) > -1) {
        return;
    }

    if (nnz == vec->capacity) {
        double_size_boolvec(vec);
    }

    (vec->indices)[nnz] = idx;

    ++(vec->num_nonzero);
}

void set_nonzero_charvec(struct SparseCharVector* vec, unsigned idx, char val) {
    unsigned nnz = vec->num_nonzero;

    int i = find_nonzero_charvec(vec, idx);

    if (i > -1) {
        (vec->values)[i] = val;
        return;
    }

    if (nnz == vec->capacity) {
        double_size_charvec(vec);
    }

    (vec->indices)[nnz] = idx;
    (vec->values)[nnz] = val;

    ++(vec->num_nonzero);
}

void add_at_idx(struct SparseCharVector* vec, unsigned idx, unsigned char val) {
    int old_val = get_value_charvec(vec, idx);

    if (old_val == 0) {
        set_nonzero_charvec(vec, idx, val);
    } else {
        set_nonzero_charvec(vec, idx, old_val + val);
    }
}

void unset_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    int idx = find_nonzero_boolmat(mtx, row, col);
    int n = mtx->num_nonzero - idx;

    if (idx < 0) {
        return;
    }

    copy_array_unsigned(mtx->rows + idx*sizeof(unsigned), mtx->rows + (idx - 1)*sizeof(unsigned), n);
    copy_array_unsigned(mtx->cols + idx*sizeof(unsigned), mtx->cols + (idx - 1)*sizeof(unsigned), n);

    --(mtx->num_nonzero);
}

void unset_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    // There appears to be some sort of memory bug in this function
    int i = find_nonzero_boolvec(vec, idx);
    int n = vec->num_nonzero - i;

    if (i < 0) {
        return;
    }

    copy_array_unsigned(vec->indices + i*sizeof(unsigned), vec->indices + (i - 1)*sizeof(unsigned), n);

    --(vec->num_nonzero);
}

void unset_nonzero_charvec(struct SparseCharVector* vec, unsigned idx) {
    int i = find_nonzero_charvec(vec, idx);
    int n = vec->num_nonzero - i;

    if (i < 0) {
        return;
    }

    copy_array_unsigned(vec->indices + i*sizeof(unsigned), vec->indices + (i - 1)*sizeof(unsigned), n);
    copy_array_char(vec->values + i*sizeof(char), vec->values + (i - 1)*sizeof(char), n);

    --(vec->num_nonzero);
}

void free_COO_boolmat(struct COOBooleanMatrix* mtx) {
    free(mtx->rows);
    free(mtx->cols);
    free(mtx);
}

void free_boolvec(struct SparseBooleanVector* vec) {
    free(vec->indices);
    free(vec);
}

void free_charvec(struct SparseCharVector* vec) {
    free(vec->indices);
    free(vec->values);
    free(vec);
}

void matvec_mult(struct COOBooleanMatrix* A, struct SparseBooleanVector* b, struct SparseCharVector* c) {
    unsigned row, col;

    for (int i = 0; i < A->num_nonzero; ++i) {
        row = (A->rows)[i];
        col = (A->cols)[i];

        if (find_nonzero_boolvec(b, col) > -1) {
            add_at_idx(c, row, 1);
        }
    }
}
