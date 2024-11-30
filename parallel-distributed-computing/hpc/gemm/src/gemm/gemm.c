//
//  gemm.c
//  gemm
//
//  Created by Giuliano Aiello on 17/11/24.
//

#include "gemm.h"
#include <stdio.h>

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the ijk indexes order.
 */
void matmatijk(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3) {
    int i, j, k;
    for (i = 0; i < N1; ++i) {
        for (j = 0; j < N3; ++j) {
            for (k = 0; k < N2; ++k) {
                *(C + (i * ldC) + j) += (*(A + (i * ldA) + k)) * (*(B + (k * ldB) + j));
            }
        }
    }
}

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the kji indexes order.
 */
void matmatkji(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3) {
    int k, j, i;
    for (k = 0; k < N2; ++k) {
        for (j = 0; j < N3; ++j) {
            for (i = 0; i < N1; ++i) {
                *(C + (i * ldC) + j) += (*(A + (i * ldA) + k)) * (*(B + (k * ldB) + j));
            }
        }
    }
}

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the ikj indexes order.
 */
void matmatikj(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3) {
    int i, k, j;
    for (i = 0; i < N1; ++i) {
        for (k = 0; k < N2; ++k) {
            for (j = 0; j < N3; ++j) {
                *(C + (i * ldC) + j) += (*(A + (i * ldA) + k)) * (*(B + (k * ldB) + j));
            }
        }
    }
}

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the jik indexes order.
 */
void matmatjik(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3) {
    int j, i, k;
    for (j = 0; j < N3; ++j) {
        for (i = 0; i < N1; ++i) {
            for (k = 0; k < N2; ++k) {
                *(C + (i * ldC) + j) += (*(A + (i * ldA) + k)) * (*(B + (k * ldB) + j));
            }
        }
    }
}

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the kij indexes order.
 */
void matmatkij(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3) {
    int k, i, j;
    for (k = 0; k < N2; ++k) {
        for (i = 0; i < N1; ++i) {
            for (j = 0; j < N3; ++j) {
                *(C + (i * ldC) + j) += (*(A + (i * ldA) + k)) * (*(B + (k * ldB) + j));
            }
        }
    }
}

/**
 @brief Performs a GeMM operation (`C`=`C`+`A``B`) in the jki indexes order.
 */
void matmatjki(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3) {
    int j, k, i;
    for (j = 0; j < N3; ++j) {
        for (k = 0; k < N2; ++k) {
            for (i = 0; i < N1; ++i) {
                *(C + (i * ldC) + j) += (*(A + (i * ldA) + k)) * (*(B + (k * ldB) + j));
            }
        }
    }
}
