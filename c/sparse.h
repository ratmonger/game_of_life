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

struct SparseBooleanVector* init_boolvec(unsigned length, unsigned init_capacity);

struct SparseCharVector* init_charvec(unsigned length, unsigned init_capacity);

int get_value_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);

int get_value_boolvec(struct SparseBooleanVector* vec, unsigned idx);

int get_value_charvec(struct SparseCharVector* vec, unsigned idx);

void set_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);

void set_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);

void set_value_charvec(struct SparseCharVector* vec, unsigned idx, unsigned char val);

int unset_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);

int unset_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);

int unset_value_charvec(struct SparseCharVector* vec, unsigned idx);

void free_COO_boolmat(struct COOBooleanMatrix* mtx);

void free_boolvec(struct SparseBooleanVector* vec);

void free_charvec(struct SparseCharVector* vec);

void matvec_mult(struct COOBooleanMatrix* A, struct SparseBooleanVector* b, struct SparseCharVector* c);

struct COOBooleanMatrix* generate_COO_stencil_matrix(unsigned n);

void print_COO_boolmat(struct COOBooleanMatrix* mtx);

void zero_out(struct SparseCharVector* vec);
