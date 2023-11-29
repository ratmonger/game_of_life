#include <stdio.h>
#include "sparse.h"
#include "gol_util.h"


int main() {
   unsigned N = 10;
   unsigned NUM_STEPS = 30;

   struct COOBooleanMatrix* update_mtx;
   struct SparseBooleanVector* glider;
   struct SparseBooleanVector* state;
   struct SparseCharVector* neighbors;

   state = init_boolvec(N*N, 16);
   neighbors = init_charvec(N*N, 16);
   glider = glider_sparse();

   embed_sparse(glider, state, 0, 0);

   update_mtx = generate_COO_stencil_matrix(N);

   for (unsigned i = 0; i < NUM_STEPS; ++i) {
       matvec_mult(update_mtx, state, neighbors);

       print_grid_sparse(state);
       printf("\n");

       /*
       print_neighbors(neighbors);
       printf("\n");
       */

       update_state_sparse(state, neighbors);
       zero_out(neighbors);
   }

}
