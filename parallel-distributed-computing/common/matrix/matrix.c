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

void init_matrix_float(const int N, const int LD, float *A){
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            A[(i * LD) + j] = 0;
        }
    }
}

void init_matrix_double(const int N, const int LD, double *A){
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            A[(i * LD) + j] = 0;
        }
    }
}

void init_matrix_sequentially_float(const int N, const int LD, float *A){
    int i, j;
    int element = 0;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            A[(i * LD) + j] = element++;
        }
    }
}

void init_matrix_sequentially_double(const int N, const int LD, double *A){
    int i, j;
    int element = 0;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            A[(i * LD) + j] = element++;
        }
    }
}

void init_matrix_randomly_float(const int N, const int LD, float *A){
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            A[(i * LD) + j] = (rand() % 100);
        }
    }
}

void init_matrix_randomly_double(const int N, const int LD, double *A){
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            A[(i * LD) + j] = (rand() % 100);
        }
    }
}

void print_matrix_float(const int N, const int LD, const float *A){
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            printf("%f ", A[(i * LD) + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_matrix_double(const int N, const int LD, const double *A){
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            printf("%f ", A[(i * LD) + j]);
        }
        printf("\n");
    }
    printf("\n");
}
