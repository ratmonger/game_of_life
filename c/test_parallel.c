#include <stdlib.h>
#include <stdio.h>
#include "gol_util.h"
#include "parallel.h"

#define TRUE 1
#define FALSE 0

unsigned array_equal(unsigned char* a, unsigned char* b, unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return FALSE;
        }
    }

    return TRUE;
}

void fill_array(unsigned char* arr, unsigned n, char val) {
    for (unsigned i = 0; i < n; ++i) {
        arr[i] = val;
    }
}

unsigned test_partitions() {
    unsigned n = 1;
    unsigned N = n + 2;
    unsigned topEq, botEq, leftEq, rightEq, intrEq;
    unsigned topLEq, topREq, botLEq, botREq;
    unsigned char* expectedEdge = malloc(n*sizeof(char));
    unsigned char* expectedIntr = malloc(n*n*sizeof(char));
    unsigned char* arr = malloc(N*N*sizeof(char));

    fill_array(arr, N*N, 1);
    fill_array(expectedEdge, n, 1);
    fill_array(expectedIntr, n*n, 1);

    struct AugmentedDomain* parts = partitions(arr, n);

    topEq = array_equal(parts->top, expectedEdge, n);
    botEq = array_equal(parts->bottom, expectedEdge, n);
    leftEq = array_equal(parts->left, expectedEdge, n);
    rightEq = array_equal(parts->right, expectedEdge, n);

    intrEq = array_equal(parts->interior, expectedIntr, n*n);

    topLEq = (parts->topL == 1);
    topREq = (parts->topR == 1);
    botLEq = (parts->botL == 1);
    botREq = (parts->botR == 1);

    return topEq && botEq && leftEq && rightEq && intrEq &&
            topLEq && topREq && botLEq && botREq;
}

unsigned char test_count_neighbors_filled() {
    unsigned n = 3;
    unsigned N = n + 2;
    unsigned char* arr = malloc(N*N*sizeof(char));

    fill_array(arr, N*N, 1);

    struct AugmentedDomain* parts = partitions(arr, n);

    unsigned char neighbors;

    for (unsigned i = 0; i < n*n; ++i) {
        neighbors = count_neighbors_parallel(parts, n, i);

        if (neighbors != 8) {
            return FALSE;
        }
    }

    return TRUE;
}

unsigned char test_count_neighbors_block() {
    unsigned n = 2;
    unsigned N = n + 2;
    unsigned char* arr = malloc(N*N*sizeof(char));
    unsigned char* block = block_dense();

    fill_array(arr, N*N, 0);
    embed_dense(block, arr, n*n, N*N, 1, 1);

    struct AugmentedDomain* parts = partitions(arr, n);

    unsigned char neighbors;

    for (unsigned i = 0; i < n*n; ++i) {
        neighbors = count_neighbors_parallel(parts, n, i);

        if (neighbors != 3) {
            printf("\tExpected %d neighbors but got %d\n", 3, neighbors);
            return FALSE;
        }
    }

    return TRUE;
}


unsigned test_count_neighbors() {
    unsigned filled_res, block_res;

    printf("Testing count_neighbors...\n");

    filled_res = test_count_neighbors_filled();
    block_res = test_count_neighbors_block();

    printf("\tFilled: %d\n", filled_res);
    printf("\tBlock: %d\n", block_res);

    return filled_res && block_res;
}

unsigned char test_update_empty() {
    unsigned n = 3;
    unsigned N = n + 2;
    unsigned char* arr = malloc(N*N*sizeof(char));
    unsigned char* expected = malloc(n*n*sizeof(char));

    fill_array(arr, N*N, 1);

    struct AugmentedDomain* parts = partitions(arr, n);

    update_state_parallel(parts, n);

    return array_equal(parts->interior, expected, n);
}


unsigned char test_update_block() {
    unsigned n = 2;
    unsigned N = n + 2;
    unsigned char* arr = malloc(N*N*sizeof(char));
    unsigned char* block = block_dense();

    fill_array(arr, N*N, 0);
    embed_dense(block, arr, 4, N*N, 1, 1);

    struct AugmentedDomain* parts = partitions(arr, n);

    // printf("Before update:\n\n");
    // print_grid_dense(parts->interior, n);
    // printf("\n");

    update_state_parallel(parts, n);

    // printf("After update:\n\n");
    // print_grid_dense(parts->interior, n);

    return array_equal(parts->interior, block, 4);
}

unsigned test_update() {
    printf("Testing update state...\n");
    unsigned char empty_res, block_res;

    empty_res = test_update_empty();
    block_res = test_update_block();

    printf("\tUpdate empty: %d\n", empty_res);
    printf("\tUpdate block: %d\n", block_res);

    return empty_res && block_res;
}


int main() {
    printf("Partitions: %d\n", test_partitions());
    printf("Neighbors: %d\n", test_count_neighbors());
    printf("Update: %d\n", test_update());
}
