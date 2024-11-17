/******************************************************************************
 * Library Name: `matrix`
 * Overview: this library provides functions for basic matrix operations.
 * Version: 1.0.0
 * Author: Giuliano Aiello
 * Created: 2024-10-11
 * Updated: 2024-10-27
 * License: MIT
 *
 */

#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

void init_matrix_randomly(const int N, const int LD, double *A){
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            A[(i * LD) + j] = (rand() % 100);
        }
    }
}

void print_matrix(const int N, const int LD, const float *A){
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            printf("%f ", A[(i * LD) + j]);
        }
        printf("\n");
    }
}
