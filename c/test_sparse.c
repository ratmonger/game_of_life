#include <stdio.h>
#include "sparse.h"
#include "gol_util.h"

#define TRUE 1
#define FALSE 0

unsigned test_init_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1, 1, 1);

    if (mtx->capacity == 1 && mtx->num_nonzero == 0) {
        free_COO_boolmat(mtx);
        return TRUE;
    } else {
        free_COO_boolmat(mtx);
        return FALSE;
    }

}

unsigned test_init_boolvec() {
    struct SparseBooleanVector* vec = init_boolvec(1, 1);

    if (vec->capacity == 1 && vec->num_nonzero == 0) {
        free_boolvec(vec); 
        return TRUE;
    } else {
        free_boolvec(vec); 
        return FALSE;
    }
}

unsigned test_init_charvec() {
    struct SparseCharVector* vec = init_charvec(1, 1);

    if (vec->capacity == 1 && vec->num_nonzero == 0) {
        free_charvec(vec); 
        return TRUE;
    } else {
        free_charvec(vec); 
        return FALSE;
    }
}

unsigned test_set_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1, 1, 1);

    set_nonzero_boolmat(mtx, 0, 0);

    if (mtx->num_nonzero == 1 && (mtx->row_indices)[0] == 0 && (mtx->col_indices)[0] == 0) {
        free_COO_boolmat(mtx);
        return TRUE;
    } else {
        free_COO_boolmat(mtx);
        return FALSE;
    }
}

unsigned test_set_boolvec() {
    struct SparseBooleanVector* vec = init_boolvec(1, 1);

    set_nonzero_boolvec(vec, 0);

    if (vec->num_nonzero == 1 && (vec->indices)[0] == 0) {
        free_boolvec(vec);
        return TRUE;
    } else {
        free_boolvec(vec);
        return FALSE;
    }
    return FALSE;
}

unsigned test_set_charvec() {
    struct SparseCharVector* vec = init_charvec(1, 1);

    set_value_charvec(vec, 0, 2);

    if (vec->num_nonzero == 1 && (vec->indices)[0] == 0) {
        free_charvec(vec);
        return TRUE;
    } else {
        free_charvec(vec);
        return FALSE;
    }
    return FALSE;
}

unsigned test_unset_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(1, 1, 1);

    set_nonzero_boolmat(mtx, 0, 0);
    unset_nonzero_boolmat(mtx, 0, 0);

    if (mtx->num_nonzero == 0 && get_value_boolmat(mtx, 0, 0) == 0) {
        free_COO_boolmat(mtx);
        return TRUE;
    } else {
        free_COO_boolmat(mtx);
        return FALSE;
    }
}

unsigned test_unset_boolvec() {
    struct SparseBooleanVector* vec = init_boolvec(1, 1);

    set_nonzero_boolvec(vec, 0);
    unset_nonzero_boolvec(vec, 0);

    if (vec->num_nonzero == 0 && get_value_boolvec(vec, 0) == 0) {
        free_boolvec(vec);
        return TRUE;
    } else {
        free_boolvec(vec);
        return FALSE;
    }
}

unsigned test_unset_charvec() {
    struct SparseCharVector* vec = init_charvec(1, 1);

    set_value_charvec(vec, 0, 2);
    unset_value_charvec(vec, 0);

    if (vec->num_nonzero == 0 && get_value_charvec(vec, 0) == 0) {
        free_charvec(vec);
        return TRUE;
    } else {
        free_charvec(vec);
        return FALSE;
    }
}

unsigned test_double_capacity_boolmat() {
    struct COOBooleanMatrix* mtx = init_COO_boolmat(2, 2, 1);

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

unsigned test_double_capacity_boolvec() {
    struct SparseBooleanVector* vec = init_boolvec(2, 1);

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

unsigned test_double_capacity_charvec() {
    struct SparseCharVector* vec = init_charvec(2, 1);

    set_value_charvec(vec, 0, 2);
    set_value_charvec(vec, 1, 2);

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
    unsigned dbl_res = test_double_capacity_boolmat();

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
    unsigned dbl_res = test_double_capacity_boolvec();

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
    unsigned dbl_res = test_double_capacity_charvec();

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
    struct COOBooleanMatrix* A = init_COO_boolmat(2, 2, 2);
    struct SparseBooleanVector* b = init_boolvec(2, 2);
    struct SparseCharVector* c = init_charvec(2, 2);

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
    struct COOBooleanMatrix* A = init_COO_boolmat(2, 2, 2);
    struct SparseBooleanVector* b = init_boolvec(2, 2);
    struct SparseCharVector* c = init_charvec(2, 2);

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
    struct COOBooleanMatrix* A = init_COO_boolmat(2, 2, 2);
    struct SparseBooleanVector* b = init_boolvec(2, 2);
    struct SparseCharVector* c = init_charvec(2, 2);

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


unsigned test_stencil_matrix() {
    unsigned rows = 5;
    unsigned result = TRUE;
    struct COOBooleanMatrix* stencil = generate_COO_stencil_matrix(rows);
    struct SparseBooleanVector* cells = init_boolvec(rows*rows, rows*rows);
    struct SparseCharVector* neighbors = init_charvec(rows*rows, rows*rows);

    print_COO_boolmat(stencil);

    for (unsigned i = 0; i < rows*rows; ++i) {
        set_nonzero_boolvec(cells, i);
    }

    matvec_mult(stencil, cells, neighbors);

    print_neighbors_sparse(neighbors);

    unsigned on_left_edge, on_right_edge, on_top_edge, on_bottom_edge;
    unsigned char expected;

    for (unsigned i = 0; i < rows*rows; ++i) {
        on_left_edge = FALSE;
        on_right_edge = FALSE;
        on_top_edge = FALSE;
        on_bottom_edge = FALSE;

        if (i % rows == 0)
            on_left_edge = TRUE;
        else if (i % rows == rows - 1)
            on_right_edge = TRUE;
        if (i / rows == 0)
            on_top_edge = TRUE;
        else if (i / rows == rows - 1)
            on_bottom_edge = TRUE;

        if (on_left_edge && on_top_edge)
            expected  = 3;
        else if (on_left_edge && on_bottom_edge)
            expected  = 3;
        else if (on_right_edge && on_top_edge)
            expected  = 3;
        else if (on_right_edge && on_bottom_edge)
            expected  = 3;
        else if (on_left_edge || on_right_edge || on_top_edge || on_bottom_edge)
            expected = 5;
        else
            expected = 8;

        if (expected != get_value_charvec(neighbors, i)) {
            printf("\tIndex: %d; Expected %d neighbors but found %d\n", i, expected, get_value_charvec(neighbors, i));
            result = FALSE;
            break;
        }
    }

    free_COO_boolmat(stencil);
    free_boolvec(cells);
    free_charvec(neighbors);

    return result;
}

int main() {
    test_boolmat();
    test_boolvec();
    test_charvec();
    test_matvec();

    printf("Testing stencil generation...\n");

    if (test_stencil_matrix())
        printf("Stencil generation PASSED!\n");
    else
        printf("Stencil generation FAILED!\n");
}
