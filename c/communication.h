#include "parallel.h"


/* Perform a single round of communication (sending and receiving) with the lefthand neighbor,
 * and store the received neighbors in grid->left. If no lefthand neighbor exists,
 * does nothing and causes no errors. Assumes a square number of processes. */
void communicate_left(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);


/* Perform a single round of communication (sending and receiving) with the righthand neighbor,
 * and store the received neighbors in grid->right. If no righthand neighbor exists,
 * does nothing and causes no errors. Assumes a square number of processes. */
void communicate_right(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);


/* Perform a single round of communication (sending and receiving) with the above neighbor,
 * and store the received neighbors in grid->top. If no above neighbor exists,
 * does nothing and causes no errors. Assumes a square number of processes. */
void communicate_above(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);


/* Perform a single round of communication (sending and receiving) with the below neighbor,
 * and store the received neighbors in grid->bottom. If no below neighbor exists,
 * does nothing and causes no errors. Assumes a square number of processes. */
void communicate_below(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);


/* Perform a single round of communication (sending and receiving) with the upper left
 * diagonal neighbor, and store the received neighbor in grid->topL. If no upper left neighbor
 * exists, does nothing and causes no errors. Assumes a square number of processes. */
void communicate_upper_left(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);


/* Perform a single round of communication (sending and receiving) with the upper right
 * diagonal neighbor, and store the received neighbor in grid->topR. If no upper right neighbor
 * exists, does nothing and causes no errors. Assumes a square number of processes. */
void communicate_upper_right(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);


/* Perform a single round of communication (sending and receiving) with the lower right
 * diagonal neighbor, and store the received neighbor in grid->botR. If no lower right neighbor
 * exists, does nothing and causes no errors. Assumes a square number of processes. */
void communicate_lower_right(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);


/* Perform a single round of communication (sending and receiving) with the lower left
 * diagonal neighbor, and store the received neighbor in grid->botL. If no lower left neighbor
 * exists, does nothing and causes no errors. Assumes a square number of processes. */
void communicate_lower_left(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);


/* Perform a single round of communication (sending and receiving) with all neighbors, and store
 * the received neighbors on the provided AugmentedDomain object. Assumes a square number of processes. */
void communicate_edges(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);


unsigned get_left_proc_circular(unsigned rank, unsigned proc_rows);


unsigned get_right_proc_circular(unsigned rank, unsigned proc_rows);


unsigned get_above_proc_circular(unsigned rank, unsigned proc_rows);


unsigned get_below_proc_circular(unsigned rank, unsigned proc_rows);


unsigned get_upper_left_proc_circular(unsigned rank, unsigned proc_rows);


unsigned get_upper_right_proc_circular(unsigned rank, unsigned proc_rows);


unsigned get_lower_right_proc_circular(unsigned rank, unsigned proc_rows);


unsigned get_lower_left_proc_circular(unsigned rank, unsigned proc_rows);


void communicate_edges_toroidal(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);
