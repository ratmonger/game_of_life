#include <stdlib.h>
#include <stdio.h>

struct COOBooleanMatrix {
    unsigned num_rows;
    unsigned num_cols;
    unsigned* row_indices;
    unsigned* col_indices;
    unsigned num_nonzero;
    unsigned capacity;
};

struct SparseBooleanVector {
    unsigned length;
    unsigned* indices;
    unsigned num_nonzero;
    unsigned capacity;
};

struct SparseCharVector {
    unsigned length;
    unsigned* indices;
    unsigned char* values;
    unsigned num_nonzero;
    unsigned capacity;
};

struct COOBooleanMatrix* init_COO_boolmat(unsigned rows, unsigned cols, unsigned init_capacity) {
    struct COOBooleanMatrix* mtx = malloc(sizeof(struct COOBooleanMatrix));
    mtx->num_rows = rows;
    mtx->num_cols = cols;
    mtx->row_indices = malloc(init_capacity*sizeof(unsigned));
    mtx->col_indices = malloc(init_capacity*sizeof(unsigned));
    mtx->num_nonzero = 0;
    mtx->capacity = init_capacity;

    return mtx;
}

struct SparseBooleanVector* init_boolvec(unsigned length, unsigned init_capacity) {
    struct SparseBooleanVector* vec = malloc(sizeof(struct SparseBooleanVector));
    vec->length = length;
    vec->indices = malloc(init_capacity*sizeof(unsigned));
    vec->num_nonzero = 0;
    vec->capacity = init_capacity;

    return vec;
}

struct SparseCharVector* init_charvec(unsigned length, unsigned init_capacity) {
    struct SparseCharVector* vec = malloc(sizeof(struct SparseCharVector));
    vec->length = length;
    vec->indices = malloc(init_capacity*sizeof(unsigned));
    vec->values = malloc(init_capacity*sizeof(char));
    vec->num_nonzero = 0;
    vec->capacity = init_capacity;

    return vec;
}

void print_array_unsigned(unsigned* arr, unsigned len) {
    for (unsigned i = 0; i < len; ++i)
        printf("%d ", arr[i]);	
    printf("\n");
}

void print_array_char(unsigned char* arr, unsigned len) {
    for (unsigned i = 0; i < len; ++i)
        printf("%d ", arr[i]);	
    printf("\n");
}

int find_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    if (row >= mtx->num_rows || col >= mtx->num_cols)
        return -1;

    for (int i = 0; i < mtx->num_nonzero; ++i) {
        if ((mtx->row_indices)[i] == row && (mtx->col_indices)[i] == col) {
            return i;
        }
    }

    return -1;
}

int get_value_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    if (row >= mtx->num_rows || col >= mtx->num_cols)
        return -1;

    for (int i = 0; i < mtx->num_nonzero; ++i) {
        if ((mtx->row_indices)[i] == row && (mtx->col_indices)[i] == col) {
            return 1;
        }
    }

    return 0;
}

int find_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    if (idx >= vec->length)
        return -1;

    for (int i = 0; i < vec->num_nonzero; ++i) {
        if ((vec->indices)[i] == idx) {
            return i;
        }
    }

    return -1;
}

int get_value_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    if (idx >= vec->length)
        return -1;

    for (int i = 0; i < vec->num_nonzero; ++i) {
        if ((vec->indices)[i] == idx) {
            return 1;
        }
    }

    return 0;
}

int find_value_charvec(struct SparseCharVector* vec, unsigned idx) {
    if (idx >= vec->length)
        return -1;

    for (int i = 0; i < vec->num_nonzero; ++i) {
        if ((vec->indices)[i] == idx) {
            return i;
        }
    }

    return -1;
}

char get_value_charvec(struct SparseCharVector* vec, unsigned idx) {
    if (idx >= vec->length)
        return -1;

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

void left_shift_array_unsigned(unsigned* arr, unsigned int n, unsigned idx) {
    for (int i = 0; i < n; ++i) {
	arr[idx + i] = arr[idx + i + 1];
    }
}

void left_shift_array_char(unsigned char* arr, unsigned int n, unsigned idx) {
    for (int i = 0; i < n; ++i) {
	arr[idx + i] = arr[idx + i + 1];
    }
}

void double_capacity_boolmat(struct COOBooleanMatrix* mtx) {
    unsigned* rows_doubled = malloc(2*(mtx->capacity)*sizeof(unsigned));
    unsigned* cols_doubled = malloc(2*(mtx->capacity)*sizeof(unsigned));

    copy_array_unsigned(mtx->row_indices, rows_doubled, mtx->num_nonzero);
    copy_array_unsigned(mtx->col_indices, cols_doubled, mtx->num_nonzero);

    free(mtx->row_indices);
    free(mtx->col_indices);

    mtx->row_indices = rows_doubled;
    mtx->col_indices = cols_doubled;
    mtx->capacity *= 2;
}

void double_capacity_boolvec(struct SparseBooleanVector* vec) {
    unsigned* indices_doubled = malloc(2*(vec->capacity)*sizeof(unsigned));

    copy_array_unsigned(vec->indices, indices_doubled, vec->num_nonzero);

    free(vec->indices);

    vec->indices = indices_doubled;

    vec->capacity *= 2;
}

void double_capacity_charvec(struct SparseCharVector* vec) {
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

int set_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    if (row >= (mtx->num_rows) || col >= (mtx->num_cols))
        return -1;

    unsigned nnz = mtx->num_nonzero;

    if (find_nonzero_boolmat(mtx, row, col) > -1) {
        return 0;
    }

    if (nnz == mtx->capacity) {
        double_capacity_boolmat(mtx);
    }

    (mtx->row_indices)[nnz] = row;
    (mtx->col_indices)[nnz] = col;

    ++(mtx->num_nonzero);

    return 0;
}

int set_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    if (idx >= vec->length)
        return -1;

    unsigned nnz = vec->num_nonzero;

    if (find_nonzero_boolvec(vec, idx) > -1) {
        return 0;
    }

    if (nnz == vec->capacity) {
        double_capacity_boolvec(vec);
    }

    (vec->indices)[nnz] = idx;

    ++(vec->num_nonzero);

    return 0;
}

int unset_value_charvec(struct SparseCharVector* vec, unsigned idx) {
    if (idx >= vec->length)
        return -1;

    int i = find_value_charvec(vec, idx);
    int n = vec->num_nonzero - i - 1;

    if (i < 0) {
        return 0;
    }

    left_shift_array_unsigned(vec->indices, n, i);
    left_shift_array_char(vec->values, n, i);

    --(vec->num_nonzero);

    return 0;
}

int set_value_charvec(struct SparseCharVector* vec, unsigned idx, char val) {
    if (idx >= vec->length)
        return -1;

    if (val == 0) {
        unset_value_charvec(vec, idx);
        return 0;
    }

    unsigned nnz = vec->num_nonzero;
    int i = find_value_charvec(vec, idx);

    if (i > -1) {
        (vec->values)[i] = val;
        return 0;
    }

    if (nnz == vec->capacity) {
        double_capacity_charvec(vec);
    }

    (vec->indices)[nnz] = idx;
    (vec->values)[nnz] = val;

    ++(vec->num_nonzero);

    return 0;
}

int add_at_idx(struct SparseCharVector* vec, unsigned idx, unsigned char val) {
    if (idx >= vec->length)
        return -1;

    int old_val = get_value_charvec(vec, idx);

    if (old_val == 0) {
        set_value_charvec(vec, idx, val);
    } else {
        set_value_charvec(vec, idx, old_val + val);
    }

    return 0;
}

int unset_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col) {
    if (row >= mtx->num_rows || col >= mtx->num_cols)
        return -1;

    int idx = find_nonzero_boolmat(mtx, row, col);
    int n = mtx->num_nonzero - idx - 1;

    if (idx < 0) {
        return 0;
    }

    left_shift_array_unsigned(mtx->row_indices, n, idx);
    left_shift_array_unsigned(mtx->col_indices, n, idx);

    --(mtx->num_nonzero);

    return 0;
}

int unset_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx) {
    if (idx >= vec->length)
        return -1;


    int i = find_nonzero_boolvec(vec, idx);

    if (i < 0) {
        return 0;
    }

    int n = vec->num_nonzero - i - 1;

    left_shift_array_unsigned(vec->indices, n, i);

    --(vec->num_nonzero);

    return 0;
}

void free_COO_boolmat(struct COOBooleanMatrix* mtx) {
    free(mtx->row_indices);
    free(mtx->col_indices);
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
        row = (A->row_indices)[i];
        col = (A->col_indices)[i];

        if (get_value_boolvec(b, col) == 1) {
            add_at_idx(c, row, 1);
        }
    }
}

void save_boolvec(struct SparseBooleanVector* vec, char* fname) {
    FILE *fptr;

    fptr = fopen(fname, "w");

    for (int i = 0; i < vec->num_nonzero; ++i) {
        fputc((vec->indices)[i], fptr);
        fputc(' ', fptr);
    }

    fclose(fptr);
}

struct COOBooleanMatrix* generate_COO_stencil_matrix(unsigned n) {
    /* Initializes a stencil matrix for an n x n grid. Note that the
     * dimensions of the matrix are n*n x n*n.
    */
    unsigned on_left_edge, on_right_edge, on_top_edge, on_bottom_edge;
    unsigned cell_row, cell_col;
    unsigned rows = n*n;

    struct COOBooleanMatrix* mtx = init_COO_boolmat(rows, rows, 8*rows);

    for (unsigned i = 0; i < rows; i++){
        cell_row = i / n;
        cell_col = i % n;

        on_left_edge = (cell_col == 0);
        on_right_edge = (cell_col == n - 1);
        on_top_edge = (cell_row == 0);
        on_bottom_edge = (cell_row == n - 1);

        // straight neighbors
        if (!on_left_edge)
            set_nonzero_boolmat(mtx, i, i-1);

        if (!on_right_edge)
            set_nonzero_boolmat(mtx, i, i+1);

        if (!on_top_edge)
            set_nonzero_boolmat(mtx, i, i - n);

        if (!on_bottom_edge)
            set_nonzero_boolmat(mtx, i, i + n);

        // diagonal neighbors
        if ((!on_left_edge) && (!on_top_edge))
            set_nonzero_boolmat(mtx, i, i - n - 1);

        if ((!on_left_edge) && (!on_bottom_edge))
            set_nonzero_boolmat(mtx, i, i + n - 1);

        if ((!on_right_edge) && (!on_top_edge))
            set_nonzero_boolmat(mtx, i, i - n + 1);

        if ((!on_right_edge) && (!on_bottom_edge))
            set_nonzero_boolmat(mtx, i, i + n + 1);
    }

    return mtx;
}

void print_COO_boolmat(struct COOBooleanMatrix* mtx) {
    for (unsigned i = 0; i < mtx->num_rows; ++i) {
        for (unsigned j = 0; j < mtx->num_cols; ++j) {
            if (find_nonzero_boolmat(mtx, i, j) > -1)
                putchar('1');
            else
                putchar('0');
            putchar(' ');
        }
        putchar('\n');
    }
}

void zero_out(struct SparseCharVector* vec) {
    vec->num_nonzero = 0;
}


void inspect_boolvec(struct SparseBooleanVector* vec) {
/* Print the actual contents of vector struct, rather than the vector it represents */
    printf("SparseBooleanVector:\n");
    printf("\tlength: %d\n", vec->length);
    printf("\tindices: ");
    print_array_unsigned(vec->indices, vec->capacity);
    printf("\tnum_nonzero: %d\n", vec->num_nonzero);
    printf("\tcapacity: %d", vec->capacity);
}

void inspect_charvec(struct SparseCharVector* vec) {
/* Print the actual contents of vector struct, rather than the vector it represents */
    printf("SparseCharVector:\n");
    printf("\tlength: %d\n", vec->length);
    printf("\tindices: ");
    print_array_unsigned(vec->indices, vec->capacity);
    printf("\tvalues: ");
    print_array_char(vec->values, vec->capacity);
    printf("\tnum_nonzero: %d\n", vec->num_nonzero);
    printf("\tcapacity: %d", vec->capacity);
}

struct COOBooleanMatrix* COO_boolmat_from_dense(unsigned char** arr, unsigned rows, unsigned cols) {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(rows, cols, 1);

    for (unsigned i = 0; i < rows; ++i) {
        for(unsigned j = 0; j < cols; ++j) {
            if(arr[i][j] != 0)
                set_nonzero_boolmat(mtx, i, j);
        }
    }

    return mtx;
}

struct SparseBooleanVector* sparse_boolvec_from_dense(unsigned char* arr, unsigned len) {
    struct SparseBooleanVector* vec = init_boolvec(len, 1);

    for (unsigned i = 0; i < len; ++i) {
        if(arr[i] != 0) {
            set_nonzero_boolvec(vec, i);
        }
    }

    return vec;
}


struct SparseCharVector* sparse_charvec_from_dense(unsigned char* arr, unsigned len) {
    struct SparseCharVector* vec = init_charvec(len, 1);

    for (unsigned i = 0; i < len; ++i) {
        if(arr[i] != 0) {
            set_value_charvec(vec, i, arr[i]);
        }
    }

    return vec;
}
