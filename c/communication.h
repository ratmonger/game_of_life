#include "parallel.h"

void communicate_left(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);

void communicate_right(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);

void communicate_above(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);

void communicate_below(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);

void communicate_upper_left(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);

void communicate_upper_right(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);

void communicate_lower_right(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);

void communicate_lower_left(struct AugmentedDomain* grid, unsigned long grid_width, struct DomainEdges* edges, unsigned rank, unsigned num_procs);
