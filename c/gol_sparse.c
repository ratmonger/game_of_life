#include <stdio.h>
#include "sparse.h"
#include "gol_util.h"


int main() {
   unsigned N = 10;
   unsigned NUM_STEPS = 5;

   struct COOBooleanMatrix* update_mtx;
   struct SparseBooleanVector* glider;
   struct SparseBooleanVector* state;
   struct SparseCharVector* neighbors;

   state = init_boolvec(N*N, 16);
   neighbors = init_charvec(N*N, 16);
   glider = generate_glider();

   embed(glider, state, 0, 0);

   update_mtx = generate_COO_stencil_matrix(N);

   for (unsigned i = 0; i < NUM_STEPS; ++i) {
       matvec_mult(update_mtx, state, neighbors);

       print_grid(state);
       printf("\n");

       print_neighbors(neighbors);
       printf("\n");

       update_state(state, neighbors);
       zero_out(neighbors);
   }

}
