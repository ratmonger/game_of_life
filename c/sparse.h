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
    char* values;
    unsigned num_nonzero;
    unsigned capacity;
};

struct COOBooleanMatrix* init_COO_boolmat(unsigned init_capacity);

struct SparseBooleanVector* init_boolvec(unsigned init_capacity);

struct SparseCharVector* init_charvec(unsigned init_capacity);

int find_nonzero_boomat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);

int find_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);

int find_nonzero_charvec(struct SparseCharVector* vec, unsigned idx);

void copy_array(unsigned* a, unsigned* b, unsigned n);

void double_size_boolmat(struct COOBooleanMatrix* mtx);

void double_size_boolvec(struct SparseBooleanVector* vec);

void double_size_charvec(struct SparseCharVector* vec);

void add_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);

void add_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);

void add_nonzero_charvec(struct SparseCharVector* vec, unsigned idx, char val);

void remove_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);

void remove_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);

void remove_nonzero_charvec(struct SparseCharVector* vec, unsigned idx);
