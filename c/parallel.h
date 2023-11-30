/* n x n local domain augmented by the edges and corners of domains on
 * neighboring processes. */
struct AugmentedDomain{
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

struct DomainEdges{
  unsigned char *top;
  unsigned char *bottom;
  unsigned char *right;
  unsigned char *left;
  unsigned char topR;
  unsigned char topL;
  unsigned char botL;
  unsigned char botR;
};

struct AugmentedDomain* init_domain(unsigned long n);

struct DomainEdges* init_edges(unsigned long n);

void get_edges(unsigned char* grid, struct DomainEdges* edges, unsigned long n);

struct AugmentedDomain* partitions(unsigned char* grid, unsigned long n);

unsigned char count_neighbors_parallel(struct AugmentedDomain* partition, unsigned long n, unsigned long i);
