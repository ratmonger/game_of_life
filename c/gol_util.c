#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sparse.h"

const char ZERO_CHAR = 48;

void print_grid(struct SparseBooleanVector* grid) {
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

void print_neighbors(struct SparseCharVector* neighbors) {
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
