struct COOBooleanMatrix {
    unsigned* rows;
    unsigned* cols;
    unsigned num_nonzero;
    unsigned capacity;
}

struct SparseBooleanVector {
    unsigned* indices;
    unsigned num_nonzero;
    unsigned capacity;
}

struct SparseCharVector {
    unsigned* indices;
    char* values;
    unsigned num_nonzero;
    unsigned capacity;
}

COOBooleanMatrix* init_COO_boolmat(unsigned init_capacity);

SparseBooleanVector* init_boolvec(unsigned init_capacity);

SparseCharVector* init_charvec(unsigned init_capacity);

int find_nonzero_boomat(COOBooleanMatrix* mtx, unsigned row, unsigned col);

int find_nonzero_boolvec(SparseBooleanVector* vec, unsigned idx);

int find_nonzero_charvec(SparseCharVector* vec, unsigned idx);

void copy_array(unsigned* a, unsigned* b, unsigned n);

void double_size_boolmat(COOBooleanMatrix* mtx);

void double_size_boolvec(SparseBooleanVector* vec);

void double_size_charvec(SparseCharVector* vec);

void add_nonzero_boolmat(COOBooleanMatrix* mtx, unsigned row, unsigned col);

void add_nonzero_boolvec(SparseBooleanVector* vec, unsigned idx);

void add_nonzero_charvec(SparseCharVector* vec, unsigned idx, char val);

void remove_nonzero_boolmat(COOBooleanMatrix* mtx, unsigned row, unsigned col);

void remove_nonzero_boolvec(SparseBooleanVector* vec, unsigned idx);

void remove_nonzero_charvec(SparseCharVector* vec, unsigned idx);
