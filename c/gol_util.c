#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sparse.h"

#define TRUE 1
#define FALSE 0

const char ZERO_CHAR = 48;

void print_grid_sparse(struct SparseBooleanVector* grid) {
    unsigned rows = (unsigned) sqrt(grid->length);
    unsigned char value;

    for (unsigned i = 0; i < rows; ++i) {
        for (unsigned j = 0; j < rows; ++j) {
            value = get_value_boolvec(grid, i*rows + j);
            putchar(value + ZERO_CHAR);
            putchar(' ');
        }
        putchar('\n');
    }
}

void print_grid_dense(char* grid, unsigned n) {
    unsigned idx;

    for (unsigned i = 0; i < n; ++i) {
        for (unsigned j = 0; j < n; ++j) {
            idx = n * i + j;
            printf("%d ", grid[idx]);
        }
        printf("\n");
    }
}

void print_neighbors_sparse(struct SparseCharVector* neighbors) {
    unsigned rows = (unsigned) sqrt(neighbors->length);
    unsigned char value;

    for (unsigned i = 0; i < rows; ++i) {
        for (unsigned j = 0; j < rows; ++j) {
            value = get_value_charvec(neighbors, i*rows + j);
            putchar(value + ZERO_CHAR);
            putchar(' ');
        }
        putchar('\n');
    }
}

struct SparseBooleanVector* make_glider_sparse() {
    struct SparseBooleanVector* glider = init_boolvec(9, 5);

    set_nonzero_boolvec(glider, 1);
    set_nonzero_boolvec(glider, 5);
    set_nonzero_boolvec(glider, 6);
    set_nonzero_boolvec(glider, 7);
    set_nonzero_boolvec(glider, 8);

    return glider;
}

void init_array(char* arr, unsigned n) {
    for (int i = 0; i < n; ++i)
            arr[i] = 0;
}

char* empty_grid_dense(unsigned n) {
    char* grid = malloc(n*n*sizeof(char));

    init_array(grid, n*n);

    return grid;
}

char* make_glider_dense() {
    char* glider = malloc(9*sizeof(char));
    init_array(glider, 9);

    glider[1] = 1;
    glider[5] = 1;
    glider[6] = 1;
    glider[7] = 1;
    glider[8] = 1;

    return glider;   
}

void embed_dense(char* u, char* v, unsigned u_size, unsigned v_size, unsigned i, unsigned j) {
    unsigned u_width = sqrt(u_size);
    unsigned v_width = sqrt(v_size);
    unsigned u_row, u_col, v_row, v_col, u_idx, v_idx;

    for (u_idx = 0; u_idx < u_size; ++u_idx) {
        u_row = u_idx / u_width;
        u_col = u_idx % u_width;

        v_row = u_row + i;
        v_col = u_col + j;
        v_idx = v_row * v_width + v_col;
        v[v_idx] = u[u_idx];
    }
}

void embed_sparse(struct SparseBooleanVector* u, struct SparseBooleanVector* v, unsigned i, unsigned j) {
    unsigned u_width = sqrt(u->length);
    unsigned v_width = sqrt(v->length);
    unsigned u_row, u_col, v_row, v_col, u_idx, v_idx;

    for (unsigned k = 0; k < u->num_nonzero; ++k) {
        u_idx = (u->indices)[k];
        u_row = u_idx / u_width;
        u_col = u_idx % u_width;

        v_row = u_row + i;
        v_col = u_col + j;
        
        v_idx = v_row * v_width + v_col;

        set_nonzero_boolvec(v, v_idx);
    }
}

void update_state_sparse(struct SparseBooleanVector* state, struct SparseCharVector* neighbors) {
    unsigned num_nbrs;

    for (unsigned i = 0; i < state->length; ++i) {
        num_nbrs = get_value_charvec(neighbors, i);

        if (num_nbrs < 2 || num_nbrs > 3) {
            unset_nonzero_boolvec(state, i);
        } else if (num_nbrs == 3) {
            set_nonzero_boolvec(state, i);
        }
    }
}
