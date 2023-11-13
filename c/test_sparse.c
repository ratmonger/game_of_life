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

unsigned test_set_boolvec() {
    struct SparseBooleanVector* vec = init_boolvec(1);

    set_nonzero_boolvec(vec, 0);

    if (vec->num_nonzero == 1 && find_nonzero_boolvec(vec, 0) == 0) {
        free_boolvec(vec);
        return TRUE;
    } else {
        free_boolvec(vec);
        return FALSE;
    }
    return FALSE;
}

unsigned test_set_charvec() {
    struct SparseCharVector* vec = init_charvec(1);

    set_nonzero_charvec(vec, 0, 2);

    if (vec->num_nonzero == 1 && find_nonzero_charvec(vec, 0) == 0) {
        free_charvec(vec);
        return TRUE;
    } else {
        free_charvec(vec);
        return FALSE;
    }
    return FALSE;
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

unsigned test_unset_boolvec() {
    struct SparseBooleanVector* vec = init_boolvec(1);

    set_nonzero_boolvec(vec, 0);
    unset_nonzero_boolvec(vec, 0);

    if (vec->num_nonzero == 0 && find_nonzero_boolvec(vec, 0) == -1) {
        free_boolvec(vec);
        return TRUE;
    } else {
        free_boolvec(vec);
        return FALSE;
    }
}

unsigned test_unset_charvec() {
    struct SparseCharVector* vec = init_charvec(1);

    set_nonzero_charvec(vec, 0, 2);
    unset_nonzero_charvec(vec, 0);

    if (vec->num_nonzero == 0 && find_nonzero_charvec(vec, 0) == -1) {
        free_charvec(vec);
        return TRUE;
    } else {
        free_charvec(vec);
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

unsigned test_double_size_boolvec() {
    struct SparseBooleanVector* vec = init_boolvec(1);

    set_nonzero_boolvec(vec, 0);
    set_nonzero_boolvec(vec, 1);

    if (vec->capacity == 2) {
        free_boolvec(vec);
        return TRUE;
    } else {
        free_boolvec(vec);
        return FALSE;
    }
}

unsigned test_double_size_charvec() {
    struct SparseCharVector* vec = init_charvec(1);

    set_nonzero_charvec(vec, 0, 2);
    set_nonzero_charvec(vec, 1, 2);

    if (vec->capacity == 2) {
        free_charvec(vec);
        return TRUE;
    } else {
        free_charvec(vec);
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
        printf("Sparse boolean matrix tests PASSED!\n");
    else
        printf("Sparse boolean matrix tests FAILED!\n");

    return result;
}

unsigned test_boolvec() {
    printf("Testing sparse boolean vector...\n");
    unsigned init_res = test_init_boolvec();
    unsigned set_res = test_set_boolvec();
    unsigned unset_res = test_unset_boolvec();
    unsigned dbl_res = test_double_size_boolvec();

    printf("\tInitialization: %d\n", init_res);
    printf("\tSet nonzero: %d\n", set_res);
    printf("\tUnset nonzero: %d\n", unset_res);
    printf("\tDouble size: %d\n", dbl_res);

    unsigned result = init_res * set_res * unset_res * dbl_res;

    if (result)
        printf("Sparse boolean vector tests PASSED!\n");
    else
        printf("Sparse boolean vector tests FAILED!\n");

    return result;
}

unsigned test_charvec() {
    printf("Testing sparse character vector...\n");
    unsigned init_res = test_init_charvec();
    unsigned set_res = test_set_charvec();
    unsigned unset_res = test_unset_charvec();
    unsigned dbl_res = test_double_size_charvec();

    printf("\tInitialization: %d\n", init_res);
    printf("\tSet nonzero: %d\n", set_res);
    printf("\tUnset nonzero: %d\n", unset_res);
    printf("\tDouble size: %d\n", dbl_res);

    unsigned result = init_res * set_res * unset_res * dbl_res;

    if (result)
        printf("Sparse character vector tests PASSED!\n");
    else
        printf("Sparse character vector tests FAILED!\n");

    return result;
}

unsigned test_matvec_identity() {
    struct COOBooleanMatrix* A = init_COO_boolmat(2);
    struct SparseBooleanVector* b = init_boolvec(2);
    struct SparseCharVector* c = init_charvec(2);

    // Make a 2x2 identity matrix
    set_nonzero_boolmat(A, 0, 0);
    set_nonzero_boolmat(A, 1, 1);

    set_nonzero_boolvec(b, 0);
    set_nonzero_boolvec(b, 1);

    matvec_mult(A, b, c);

    if (get_value_charvec(c, 0) == 1 && get_value_charvec(c, 1) == 1) {
        free_COO_boolmat(A);
        free_boolvec(b);
        free_charvec(c);
        return TRUE;
    } else {
        free_COO_boolmat(A);
        free_boolvec(b);
        free_charvec(c);
        return FALSE;
    }
}

unsigned test_matvec_zero() {
    struct COOBooleanMatrix* A = init_COO_boolmat(2);
    struct SparseBooleanVector* b = init_boolvec(2);
    struct SparseCharVector* c = init_charvec(2);

    set_nonzero_boolvec(b, 0);
    set_nonzero_boolvec(b, 1);

    matvec_mult(A, b, c);

    if (get_value_charvec(c, 0) == 0 && get_value_charvec(c, 1) == 0) {
        free_COO_boolmat(A);
        free_boolvec(b);
        free_charvec(c);
        return TRUE;
    } else {
        free_COO_boolmat(A);
        free_boolvec(b);
        free_charvec(c);
        return FALSE;
    }
}

unsigned test_matvec_general() {
    struct COOBooleanMatrix* A = init_COO_boolmat(2);
    struct SparseBooleanVector* b = init_boolvec(2);
    struct SparseCharVector* c = init_charvec(2);

    set_nonzero_boolmat(A, 0, 0);
    set_nonzero_boolmat(A, 0, 1);
    set_nonzero_boolmat(A, 1, 0);
    set_nonzero_boolmat(A, 1, 1);

    set_nonzero_boolvec(b, 0);
    set_nonzero_boolvec(b, 1);

    matvec_mult(A, b, c);

    if (get_value_charvec(c, 0) == 2 && get_value_charvec(c, 1) == 2) {
        free_COO_boolmat(A);
        free_boolvec(b);
        free_charvec(c);
        return TRUE;
    } else {
        free_COO_boolmat(A);
        free_boolvec(b);
        free_charvec(c);
        return FALSE;
    }
}

unsigned test_matvec() {
    printf("Testing matvec multiply...\n");

    unsigned id_res = test_matvec_identity();
    unsigned zero_res = test_matvec_zero();
    unsigned gen_res = test_matvec_general();

    printf("\tIdentity: %d\n", id_res);
    printf("\tZero: %d\n", zero_res);
    printf("\tGeneral Case: %d\n", gen_res);

    unsigned result = id_res * zero_res;

    if (result) {
        printf("matvec tests PASSED!\n");
    } else {
        printf("matvec tests FAILED!\n");
    }

    return result;
}


int main() {
    test_boolmat();
    // test_boolvec();
    test_charvec();
    test_matvec();
}
