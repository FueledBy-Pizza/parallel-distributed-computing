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

#ifndef matrix_h
#define matrix_h

void init_matrix_float(const int N, const int LD, float *A);
void init_matrix_double(const int N, const int LD, double *A);

void init_matrix_sequentially_float(const int N, const int LD, float *A);
void init_matrix_sequentially_double(const int N, const int LD, double *A);

void init_matrix_randomly_float(const int N, const int LD, float *A);
void init_matrix_randomly_double(const int N, const int LD, double *A);

void print_matrix_float(const int N, const int LD, const float *A);
void print_matrix_double(const int N, const int LD, const double *A);

#endif /* matrix_h */
