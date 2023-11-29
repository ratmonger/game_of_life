#include <stdlib.h>
#include <stdio.h>
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

    printf("Filling arrays...\n");
    fill_array(arr, N*N, 1);
    fill_array(expectedEdge, n, 1);
    fill_array(expectedIntr, n*n, 1);

    printf("Partitioning vector...\n");
    struct SrcVectors* parts = partitions(arr, n);

    printf("Testing equality...\n");
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

int main() {
    printf("Partitions: %d\n", test_partitions());
}
