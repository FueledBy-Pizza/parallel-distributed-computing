//
//  hpc.c
//  hpc
//
//  Created by Giuliano Aiello on 17/11/24.
//

#include "hpc.h"

/**
 @brief Performs a GeMM operation int the ijk indexes order.
 */
void gemm_ikj(double *C, const double *A, const double *B, const int N, const int LD) {
    int i, k, j;
    for (i = 0; i < N; ++i) {
        for (k = 0; k < N; ++k) {
            for (j = 0; j < N; ++j) {
                C[(i * LD) + j] = C[(i * LD) + j] + (A[(i * LD) + k] * B[(k * LD) + j]);
            }
        }
    }
}

/**
 @brief Performs a GeMM operation int the kji indexes order.
 */
void gemm_kji(double *C, const double *A, const double *B, const int N, const int LD) {
    int k, j, i;
    for (k = 0; k < N; ++k) {
        for (j = 0; j < N; ++j) {
            for (i = 0; i < N; ++i) {
                C[(i * LD) + j] = C[(i * LD) + j] + (A[(i * LD) + k] * B[(k * LD) + j]);
            }
        }
    }
}
