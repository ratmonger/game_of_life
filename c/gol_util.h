#include "sparse.h"

void print_grid_sparse(struct SparseBooleanVector* grid);

void print_grid_dense(char* grid, unsigned n);

void print_neighbors_sparse(struct SparseCharVector* neighbors);

void embed_sparse(struct SparseBooleanVector* u, struct SparseBooleanVector* v, unsigned i, unsigned j);

void embed_dense(char* u, char* v, unsigned u_size, unsigned v_size, unsigned i, unsigned j);

void update_state_sparse(struct SparseBooleanVector* state, struct SparseCharVector* neighbors);

char* empty_grid_dense(unsigned n);

struct SparseBooleanVector* make_glider_sparse();

char* make_glider_dense();
