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
    unsigned char* values;
    unsigned num_nonzero;
    unsigned capacity;
};

struct COOBooleanMatrix* init_COO_boolmat(unsigned init_capacity);

struct SparseBooleanVector* init_boolvec(unsigned init_capacity);

struct SparseCharVector* init_charvec(unsigned init_capacity);

int find_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);

int find_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);

int find_nonzero_charvec(struct SparseCharVector* vec, unsigned idx);

int get_value_charvec(struct SparseCharVector* vec, unsigned idx);

void copy_array_unsigned(unsigned* a, unsigned* b, unsigned n);

void copy_array_char(unsigned char* a, unsigned char* b, unsigned n);

void double_size_boolmat(struct COOBooleanMatrix* mtx);

void double_size_boolvec(struct SparseBooleanVector* vec);

void double_size_charvec(struct SparseCharVector* vec);

void set_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);

void set_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);

void set_nonzero_charvec(struct SparseCharVector* vec, unsigned idx, unsigned char val);

void add_at_idx(struct SparseCharVector* vec, unsigned idx, unsigned char val);

void unset_nonzero_boolmat(struct COOBooleanMatrix* mtx, unsigned row, unsigned col);

void unset_nonzero_boolvec(struct SparseBooleanVector* vec, unsigned idx);

void unset_nonzero_charvec(struct SparseCharVector* vec, unsigned idx);

void free_COO_boolmat(struct COOBooleanMatrix* mtx);

void free_boolvec(struct SparseBooleanVector* vec);

void free_charvec(struct SparseCharVector* vec);

void matvec_mult(struct COOBooleanMatrix* A, struct SparseBooleanVector* b, struct SparseCharVector* c);
