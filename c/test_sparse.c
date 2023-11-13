#include <stdio.h>
#include "sparse.h"

#define TRUE 1
#define FALSE 0

unsigned test_init_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1);

    if (mtx->capacity == 1 && mtx->num_nonzero == 0) {
        free_COO_boolmat(mtx);
        return TRUE;
    } else {
        free_COO_boolmat(mtx);
        return FALSE;
    }

}

unsigned test_init_boolvec() {
    struct SparseBooleanVector* vec = init_boolvec(1);

    if (vec->capacity == 1 && vec->num_nonzero == 0) {
        free_boolvec(vec); 
        return TRUE;
    } else {
        free_boolvec(vec); 
        return FALSE;
    }
}

unsigned test_init_charvec() {
    struct SparseCharVector* vec = init_charvec(1);

    if (vec->capacity == 1 && vec->num_nonzero == 0) {
        free_charvec(vec); 
        return TRUE;
    } else {
        free_charvec(vec); 
        return FALSE;
    }
}

unsigned test_set_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1);

    set_nonzero_boolmat(mtx, 0, 0);

    if (mtx->num_nonzero == 1 && find_nonzero_boolmat(mtx, 0, 0) == 0) {
        free_COO_boolmat(mtx);
        return TRUE;
    } else {
        free_COO_boolmat(mtx);
        return FALSE;
    }
}

unsigned test_unset_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1);

    set_nonzero_boolmat(mtx, 0, 0);
    unset_nonzero_boolmat(mtx, 0, 0);

    if (mtx->num_nonzero == 0 && find_nonzero_boolmat(mtx, 0, 0) == -1) {
        free_COO_boolmat(mtx);
        return TRUE;
    } else {
        free_COO_boolmat(mtx);
        return FALSE;
    }
}

unsigned test_double_size_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1);

    set_nonzero_boolmat(mtx, 0, 0);
    set_nonzero_boolmat(mtx, 0, 1);

    if (mtx->capacity == 2) {
        free_COO_boolmat(mtx);
        return TRUE;
    } else {
        free_COO_boolmat(mtx);
        return FALSE;
    }
}

unsigned test_boolmat() {
    printf("Testing sparse boolean matrix...\n");
    unsigned init_res = test_init_boolmat();
    unsigned set_res = test_set_boolmat();
    unsigned unset_res = test_unset_boolmat();
    unsigned dbl_res = test_double_size_boolmat();

    printf("\tInitialization: %d\n", init_res);
    printf("\tSet nonzero: %d\n", set_res);
    printf("\tUnset nonzero: %d\n", unset_res);
    printf("\tDouble size: %d\n", dbl_res);

    unsigned result = init_res * set_res * unset_res * dbl_res;

    if (result)
        printf("Sparse boolean matrix tests passed!\n");
    else
        printf("Sparse boolean matrix tests failed\n");

    return result;
}

int main() {
    test_boolmat();
}
