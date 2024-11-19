//
//  hpc.c
//  hpc
//
//  Created by Giuliano Aiello on 17/11/24.
//

#include "hpc.h"

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the ijk indexes order.
 */
void matmatijk(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3) {
    int i, j, k;
    for (i = 0; i < N1; ++i) {
        for (j = 0; j < N3; ++j) {
            for (k = 0; k < N2; ++k) {
                C[(i * ldC) + j] = C[(i * ldC) + j] + (A[(i * ldA) + k] * B[(k * ldB) + j]);
            }
        }
    }
}

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the kji indexes order.
 */
void matmatkji(double *C, const double *A, const double *B, const int N, const int LD) {
    int k, j, i;
    for (k = 0; k < N; ++k) {
        for (j = 0; j < N; ++j) {
            for (i = 0; i < N; ++i) {
                C[(i * LD) + j] = C[(i * LD) + j] + (A[(i * LD) + k] * B[(k * LD) + j]);
            }
        }
    }
}

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the ikj indexes order.
 */
void matmatikj(double *C, const double *A, const double *B, const int N, const int LD) {
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
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the jik indexes order.
 */
void matmatjik(double *C, const double *A, const double *B, const int N, const int LD) {
    int j, i, k;
    for (j = 0; j < N; ++j) {
        for (i = 0; i < N; ++i) {
            for (k = 0; k < N; ++k) {
                C[(i * LD) + j] = C[(i * LD) + j] + (A[(i * LD) + k] * B[(k * LD) + j]);
            }
        }
    }
}

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the kij indexes order.
 */
void matmatkij(double *C, const double *A, const double *B, const int N, const int LD) {
    int k, i, j;
    for (k = 0; k < N; ++k) {
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j) {
                C[(i * LD) + j] = C[(i * LD) + j] + (A[(i * LD) + k] * B[(k * LD) + j]);
            }
        }
    }
}

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the jki indexes order.
 */
void matmatjki(double *C, const double *A, const double *B, const int N, const int LD) {
    int j, k, i;
    for (j = 0; j < N; ++j) {
        for (k = 0; k < N; ++k) {
            for (i = 0; i < N; ++i) {
                C[(i * LD) + j] = C[(i * LD) + j] + (A[(i * LD) + k] * B[(k * LD) + j]);
            }
        }
    }
}
