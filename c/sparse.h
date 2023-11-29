#ifndef SPARSE_H
#define SPARSE_H

struct COOBooleanMatrix {
    unsigned num_rows;
    unsigned num_cols;
    unsigned* row_indices;
    unsigned* col_indices ;
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


struct COOBooleanMatrix* init_COO_boolmat(unsigned rows, unsigned cols, unsigned init_capacity);
/* Create and initialize an empty COO boolean matrix from scratch, with the specified number of
 * rows and columns. Capacity is the actual amount of space allocated, which will dynamically increase
 * as new elements are added.*/


struct SparseBooleanVector* init_boolvec(unsigned length, unsigned init_capacity);
/* Create and initialize an empty sparse boolean vector from scratch, with the specified number
 * length. Capacity is the actual amount of space allocated, which will dynamically increase
 * as new elements are added. Length, in contrast, is the fixed size of the vector,
 * including zero values.*/


struct SparseCharVector* init_charvec(unsigned length, unsigned init_capacity);
/* Create and initialize an empty sparse char vector from scratch, with the specified number of
 * length. Capacity is the actual amount of space allocated, which will dynamically increase
 * as new elements are added. Length, in contrast, is the fixed size of the vector,
 * including zero values.*/


int get_value_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);
/* Get the element at the specified row and column. The result with be either 0 or 1.
 * If the requested row or column lie outside of the matrix, returns -1. */


int get_value_boolvec(struct SparseBooleanVector* vec, unsigned idx);
/* Get the element at the specified index. The result with be either 0 or 1.
 * If the requested index exceeds the length of the vector, returns -1. */


int get_value_charvec(struct SparseCharVector* vec, unsigned idx);
/* Get the element at the specified index. All values are non-negative.
 * If the requested index exceeds the length of the vector, returns -1. */


void set_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);
/* Set the element at the specified row and column to 1 (True) */


void set_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);
/* Set the element at the specified index to 1 (True) */


void set_value_charvec(struct SparseCharVector* vec, unsigned idx, unsigned char val);
/* Set the element at the specified index to the value specified. */


int unset_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);
/* Set the element at the specified row and column to 0 (False) */


int unset_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);
/* Set the element at the specified index to 0 (False) */


int unset_value_charvec(struct SparseCharVector* vec, unsigned idx);
/* Set the element at the specified row and column to 0 */


void free_COO_boolmat(struct COOBooleanMatrix* mtx);
/* Deallocate the COO boolean matrix struct */


void free_boolvec(struct SparseBooleanVector* vec);
/* Deallocate the boolean vector struct */


void free_charvec(struct SparseCharVector* vec);
/* Deallocate the char vector struct */


void matvec_mult(struct COOBooleanMatrix* A, struct SparseBooleanVector* b, struct SparseCharVector* c);
/* Multiply a COO boolean matrix with a sparse boolean vector, resulting in a sparse char vector */


struct COOBooleanMatrix* generate_COO_stencil_matrix(unsigned n);
/* Generate a sparse stencil matrix, of size n*n by n*n, to compute the neighbors
 * at each step of the Game of Life update process.*/


void print_COO_boolmat(struct COOBooleanMatrix* mtx);
/* Print out an arbitrary COO boolean matrix */


void zero_out(struct SparseCharVector* vec);
/* Set all elements of the sparse char vector to 0. Does not actually step through elements,
 * but sets the number of nonzero elements to 0, allowing existing elements to be overwritten
 * as new elements are added. */


void inspect_boolvec(struct SparseBooleanVector* vec);
/* Print out the internals of the sparse boolean vector struct. Intended for debugging. */


void inspect_charvec(struct SparseCharVector* vec);
/* Print out the internals of the sparse char vector struct. Intended for debugging. */


#endif
