#include <stdio.h>
#include "sparse.h"

#define TRUE 1
#define FALSE 0

unsigned test_init_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1);

    if (mtx->capacity == 1 && mtx->num_nonzero == 0)
        return TRUE;
    else
        return FALSE;
}

unsigned test_init_boolvec() {
    struct SparseBooleanVector* vec = init_boolvec(1);

    if (vec->capacity == 1 && vec->num_nonzero == 0)
        return TRUE;
    else
        return FALSE;
}

unsigned test_init_charvec() {
    struct SparseCharVector* vec = init_charvec(1);

    if (vec->capacity == 1 && vec->num_nonzero == 0)
        return TRUE;
    else
        return FALSE;
}

unsigned test_add_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1);

    add_nonzero_boolmat(mtx, 0, 0);

    if (mtx->num_nonzero == 1 && find_nonzero_boolmat(mtx, 0, 0) == 0)
        return TRUE;
    else
        return FALSE;
}

unsigned test_remove_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1);

    add_nonzero_boolmat(mtx, 0, 0);
    remove_nonzero_boolmat(mtx, 0, 0);

    if (mtx->num_nonzero == 0 && find_nonzero_boolmat(mtx, 0, 0) == -1)
        return TRUE;
    else
        return FALSE;
}

unsigned test_double_size_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1);

    add_nonzero_boolmat(mtx, 0, 0);
    add_nonzero_boolmat(mtx, 0, 1);

    if (mtx->capacity == 2)
        return TRUE;
    else
        return FALSE;
}

unsigned test_boolmat() {
    printf("Testing sparse boolean matrix...\n");
    unsigned init_res = test_init_boolmat();
    unsigned add_res = test_add_boolmat();
    unsigned rmv_res = test_remove_boolmat();
    unsigned dbl_res = test_double_size_boolmat();

    printf("\tInitialization: %d\n", init_res);
    printf("\tAdd nonzero: %d\n", add_res);
    printf("\tRemove nonzero: %d\n", rmv_res);
    printf("\tDouble size: %d\n", dbl_res);

    unsigned result = init_res * add_res * rmv_res * dbl_res;

    if (result)
        printf("Sparse boolean matrix tests passed!\n");
    else
        printf("Sparse boolean matrix tests failed\n");

    return result;
}

int main() {
    test_boolmat();
}
