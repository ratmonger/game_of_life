#include "sparse.h"

void print_grid_sparse(struct SparseBooleanVector* grid);

void print_grid_dense(unsigned char* grid, unsigned n);

void print_neighbors_sparse(struct SparseCharVector* neighbors);

void embed_sparse(struct SparseBooleanVector* u, struct SparseBooleanVector* v, unsigned i, unsigned j);

void embed_dense(unsigned char* u, unsigned char* v, unsigned u_size, unsigned v_size, unsigned i, unsigned j);

unsigned char is_alive(unsigned char cell, unsigned char neighbors);

void update_state_sparse(struct SparseBooleanVector* state, struct SparseCharVector* neighbors);

unsigned char* update_state_dense(unsigned char* grid, unsigned n);

unsigned char* empty_grid_dense(unsigned n);

unsigned char* random_grid_dense(unsigned n, double density);

struct SparseBooleanVector* random_grid_sparse(unsigned n, double density);

struct SparseBooleanVector* glider_sparse();

unsigned char* glider_dense();

unsigned char* block_dense();
