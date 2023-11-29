struct SrcVectors{
    char *interior;
    char *top;
    char *bottom;
    char *right;
    char *left;
    char topR;
    char topL;
    char botL;
    char botR;
};

void init_partition(struct SrcVectors* part, unsigned n);

struct SrcVectors* partitions(unsigned char* grid, unsigned long n);
