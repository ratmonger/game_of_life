#include <stdlib.h>
#include "gol_util.h"

#define TRUE 1
#define FALSE 0


struct AugmentedDomain{
    unsigned char *interior;
    unsigned char *top;
    unsigned char *bottom;
    unsigned char *right;
    unsigned char *left;
    unsigned char topR;
    unsigned char topL;
    unsigned char botL;
    unsigned char botR;
};

struct DomainEdges{
  unsigned char *top;
  unsigned char *bottom;
  unsigned char *right;
  unsigned char *left;
  unsigned char topR;
  unsigned char topL;
  unsigned char botL;
  unsigned char botR;
};


struct AugmentedDomain* init_domain(unsigned long n) {
    struct AugmentedDomain* domain = malloc(sizeof(struct DomainEdges));

    domain->interior = malloc(n*n*sizeof(char));
    domain->top = malloc(n*sizeof(char));
    domain->bottom = malloc(n*sizeof(char));
    domain->right = malloc(n*sizeof(char));
    domain->left = malloc(n*sizeof(char));

    return domain;
}


struct DomainEdges* init_edges(unsigned long n) {
    struct DomainEdges* edges = malloc(sizeof(struct DomainEdges));

    edges->top = malloc((n-2)*sizeof(char));
    edges->bottom = malloc((n-2)*sizeof(char));
    edges->left = malloc((n-2)*sizeof(char));
    edges->right = malloc((n-2)*sizeof(char));

    return edges;
}


void copy_interior(unsigned char* A, unsigned char* B, unsigned long n) {
/* Assumes A has (n + 2)^2 elements, B has n^2 elements */
    unsigned A_row, A_col, A_idx;

    for (unsigned long i = 0; i < n*n; ++i) {
        A_row = i / n + 1;
        A_col = i % n + 1;
        A_idx = A_row * (n + 2) + A_col;

        B[i] = A[A_idx];
    }
}


void copy_row_interior(unsigned char* A, unsigned char* b, unsigned long n, unsigned long row) {
    for(unsigned long i = 0; i < n; ++i) {
        b[i] = A[row*(n + 2) + i + 1];
    }
}


void copy_col_interior(unsigned char* A, unsigned char* b, unsigned long n, unsigned long col) {
    for(unsigned long i = 0; i < n; ++i) {
        b[i] = A[(n + 2)*(i + 1) + col];
    }
}


void get_edges(unsigned char* grid, struct DomainEdges* edges, unsigned long n) {
    copy_row_interior(grid, edges->top, n, 0);
    copy_row_interior(grid, edges->bottom, n, n-1);
    copy_col_interior(grid, edges->left, n, 0);
    copy_col_interior(grid, edges->right, n, n-1);

    edges->topL = grid[0];
    edges->topR = grid[n-1];
    edges->botL = grid[n*(n-1)];
    edges->botR = grid[n*n - 1];
}


struct AugmentedDomain* partitions(unsigned char* grid, unsigned long n){
    struct AugmentedDomain* srcVec = malloc(sizeof(struct AugmentedDomain));

    srcVec = init_domain(n);

    copy_interior(grid, srcVec->interior, n);

    copy_row_interior(grid, srcVec->top, n, 0);
    copy_row_interior(grid, srcVec->bottom, n, n + 1);
    copy_col_interior(grid, srcVec->left, n, 0);
    copy_col_interior(grid, srcVec->right, n, n + 1);
    
    srcVec->topL = grid[0];
    srcVec->topR = grid[n + 1];
    srcVec->botL = grid[(n + 2) * (n + 1)];
    srcVec->botR = grid[(n + 2)*(n + 2) - 1];

    return srcVec;
}

unsigned char count_neighbors_parallel(struct AugmentedDomain* partition, unsigned long n, unsigned long i) {
    unsigned char on_left_edge, on_right_edge, on_top_edge, on_bottom_edge;
    unsigned char neighbors = 0;

    on_left_edge = (i % n == 0);
    on_right_edge = (i % n == n - 1);
    on_top_edge = (i / n == 0);
    on_bottom_edge = (i / n == n - 1);
    
    // Interior neighbors
    if (!on_left_edge && (partition->interior)[i - 1] == 1)
        ++neighbors;
    if (!on_right_edge && (partition->interior)[i + 1] == 1)
        ++neighbors;
    if (!on_top_edge && (partition->interior)[i - n] == 1)
        ++neighbors;
    if (!on_bottom_edge && (partition->interior)[i + n] == 1)
        ++neighbors;

    if (!(on_left_edge || on_top_edge) && (partition->interior)[i - n - 1] == 1)
        ++neighbors;
    if (!(on_left_edge || on_bottom_edge) && (partition->interior)[i + n - 1] == 1)
        ++neighbors;
    if (!(on_right_edge || on_top_edge) && (partition->interior)[i - n + 1] == 1)
        ++neighbors;
    if (!(on_right_edge || on_bottom_edge) && (partition->interior)[i + n + 1] == 1)
        ++neighbors;

    // Exterior neighbors
    if (on_left_edge && (partition->left)[i / n] == 1)
        ++neighbors;
    if (on_right_edge && (partition->right)[i / n] == 1)
        ++neighbors;
    if (on_top_edge && (partition->top)[i % n] == 1)
        ++neighbors;
    if (on_bottom_edge && (partition->bottom)[i % n] == 1)
        ++neighbors;

    if (on_left_edge && !on_top_edge && (partition->left)[i / n - 1] == 1)
        ++neighbors;
    if (on_left_edge && !on_bottom_edge && (partition->left)[i / n + 1] == 1)
        ++neighbors;
    if (on_right_edge && !on_top_edge && (partition->right)[i / n - 1] == 1)
        ++neighbors;
    if (on_right_edge && !on_bottom_edge && (partition->right)[i / n + 1] == 1)
        ++neighbors;
    if (on_top_edge && !on_left_edge && (partition->top)[i % n - 1] == 1)
        ++neighbors;
    if (on_top_edge && !on_right_edge && (partition->top)[i % n + 1] == 1)
        ++neighbors;
    if (on_bottom_edge && !on_left_edge && (partition->bottom)[i % n - 1] == 1)
        ++neighbors;
    if (on_bottom_edge && !on_right_edge && (partition->bottom)[i % n + 1] == 1)
        ++neighbors;

    if (on_left_edge && on_top_edge && partition->topL == 1)
        ++neighbors;
    if (on_left_edge && on_bottom_edge && partition->botL == 1)
        ++neighbors;
    if (on_right_edge && on_top_edge && partition->topR == 1)
        ++neighbors;
    if (on_right_edge && on_bottom_edge && partition->botR == 1)
        ++neighbors;

    return neighbors;
}


void update_state_parallel(struct AugmentedDomain* grid, unsigned long n) {
    unsigned char neighbors;
    unsigned char* new_grid = calloc(n*n, sizeof(char));

    for (unsigned long i = 0; i < n*n; ++i) {
        neighbors = count_neighbors_parallel(grid, n, i);
        
        if (is_alive((grid->interior)[i], neighbors))
            new_grid[i] = 1;
    }

    free(grid->interior);
    grid->interior = new_grid;
}

void free_augmented_domain(struct AugmentedDomain* domain) {
    free(domain->interior);
    free(domain->left);
    free(domain->right);
    free(domain->top);
    free(domain->bottom);
    free(domain);
}

void free_domain_edges(struct DomainEdges* edges) {
    free(edges->left);
    free(edges->right);
    free(edges->top);
    free(edges->bottom);
}

unsigned char array_empty(unsigned char* arr, unsigned long n) {
    for (unsigned long i = 0; i < n; ++i) {
        if (arr[i] != 0)
            return FALSE;
    }

    return TRUE;
}


unsigned char is_empty(struct DomainEdges* edges, unsigned long n) {
    if (!array_empty(edges->left, n - 2))
        return FALSE;
    else if (!array_empty(edges->right, n - 2))
        return FALSE;
    else if (!array_empty(edges->top, n - 2))
        return FALSE;
    else if (!array_empty(edges->bottom, n - 2))
        return FALSE;
    else if (edges->topL != 0)
        return FALSE;
    else if (edges->topR != 0)
        return FALSE;
    else if (edges->botL != 0)
        return FALSE;
    else if (edges->botR != 0)
        return FALSE;
    else
        return TRUE;
}

unsigned char count_nonzero(unsigned char* arr, unsigned long n) {
    unsigned char count = 0;

    for (unsigned long i = 0; i < n; ++i) {
        if (arr[i] > 0)
            ++count;
    }

    return count;
}

unsigned char count_exterior(struct AugmentedDomain* grid, unsigned long n) {
    unsigned char count;

    count = count_nonzero(grid->left, n - 2);
    count += count_nonzero(grid->right, n - 2);
    count += count_nonzero(grid->top, n - 2);
    count += count_nonzero(grid->bottom, n - 2);

    if (grid->topL)
        ++count;
    if (grid->topR)
        ++count;
    if (grid->botL)
        ++count;
    if (grid->botR)
        ++count;
    
    return count;
}
