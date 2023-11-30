struct SrcVectors{
    unsigned char *interior;
    unsigned char *top;
    unsigned char *bottom;
    unsigned char *right;
    unsigned char *left;
    unsigned char topR;
    unsigned char topL;
    unsigned char botL;
    unsigned char botR;
};

void init_partition(struct SrcVectors* part, unsigned n);

struct SrcVectors* partitions(unsigned char* grid, unsigned long n);

unsigned char count_neighbors_parallel(struct SrcVectors* partition, unsigned n, unsigned i);
