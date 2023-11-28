void print_grid(struct SparseBooleanVector* grid);

void print_neighbors(struct SparseCharVector* neighbors);

struct SparseBooleanVector* generate_glider();

void embed(struct SparseBooleanVector* u, struct SparseBooleanVector* v, unsigned i, unsigned j);

void update_state(struct SparseBooleanVector* state, struct SparseCharVector* neighbors);
