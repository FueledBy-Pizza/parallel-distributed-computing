//
//  matrix_ops.c
//  parallel-distributed-computing
//
//  Created by Giuliano Aiello on 11/10/24.
//

#include "matrix_ops.h"
#include <stdio.h>
#include <stdlib.h>

void init_matrix_randomly(int *N, int *LD, double **A){
    *A = (double *) malloc(sizeof(double) * (*N) * (*LD));

    int i, j;
    for (i = 0; i < (*N); ++i) {
        for (j = 0; j < (*LD); ++j) {
            (*A)[i * (*LD) + j] = (rand() % 100);
        }
    }
}

void print_matrix(int N, int LD, double *A){
    printf("Matrix is:\n\n");
    
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            printf("%f ", A[i * LD + j]);
        }
        printf("\n");
    }
}
