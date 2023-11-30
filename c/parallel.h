#ifndef PARALLEL_H
#define PARALLEL_H


/* n x n local domain augmented by the edges and corners of domains on
 * neighboring processes. interior (length n*n) represents the local grid, whereas
 * the other four char arrays represent edge data on other processes. These four
 * arrays, as well as the four single chars, are intended only for incoming
 * communication of data. */
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


/* A heterogeneous collection of the edges and corners of a local n x n grid.
 * Intended only for outgoing communication. For an n x n grid, the four char
 * arrays are each length n. */
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


/* Creates and initializes an AugmentedDomain to the approprate size. Assumes an n x n local domain.
 * Initialized values are guaranteed to be zeros. */
struct AugmentedDomain* init_domain(unsigned long n);
 

/* Creates an initializes a DomainEdges object to the appropriate size, assuming an n x n local domain.
 * Initialized values are not guaranteed to be zeros. */
struct DomainEdges* init_edges(unsigned long n);


/* Deallocates an AugmentedDomain object */
void free_augmented_domain(struct AugmentedDomain* domain);


/* Deallocates a DomainEdges object */
void free_domain_edges(struct DomainEdges* edges);


/* Extracts the edges from the n x n grid and places them into the provided edges object .*/
void get_edges(unsigned char* grid, struct DomainEdges* edges, unsigned long n);


/* Takes a grid of size (n x 2) x (n x 2), representing a local grid and its neighboring edges,
 * and separates it into the local portion and its edges, all of which are placed into the
 * appropriate fields on the AugmentedDomain object. */
struct AugmentedDomain* partitions(unsigned char* grid, unsigned long n);


/* Counts the neighbors of the cell at the index i, taking into account the non-local edges stored in the
 * AugmentedDomain object. Assumes edges have already been communicated and placed into the appropriate field. */
unsigned char count_neighbors_parallel(struct AugmentedDomain* partition, unsigned long n, unsigned long i);


/* Updates the state of AugmentedGrid interior according to the rules of the Game of Life. Assumes edges have
 * already been communicated. Does not modify edges, only the interior cells. */
void update_state_parallel(struct AugmentedDomain* grid, unsigned long n);


/* Returns TRUE if all values along the edge of a domain (represented in the DomainEdges object) are zero,
 * or FALSE otherwise. Intended for debugging. */
unsigned char is_empty(struct DomainEdges* edges, unsigned long n);


/* Counts the number of exterior neighbors (e.g. neighboring cells that exist on other processes). Assumes
 * edges have already been communicated. Intended for debugging. */
unsigned char count_exterior(struct AugmentedDomain* grid, unsigned long n);

#endif
