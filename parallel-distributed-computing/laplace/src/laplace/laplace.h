//
//  laplace.h
//  laplace
//
//  Created by Giuliano Aiello on 06/11/24.
//

#ifndef laplace_h
#define laplace_h

#include <mpi.h>

void laplace (float *A, float *Anew, float *daprev, float *danext, int N, int LD, int Niter);
void laplace_nb (float *A, float *Anew, float *daprev, float *danext, int N, int LD, int Niter);

void calculate_inner(const float *A, const int N, const int LD, float *Anew, const int N_Anew);
void calculate_row_top(const float *A, const int N, const int LD, float *Anew, const float *daprev);
void calculate_row_bottom(const float *A, const int N, const int LD, const int rows_per_proc, float *Anew, const float *danext);

float discrete_laplacian(const float *A, const int i, const int j, const int LD);
float discrete_laplacian_top(const float *A, const int j, const int LD, const float *daprev);
float discrete_laplacian_bottom(const float *A, const int i, const int j, int LD, const float *danext);

void copy_inner(float *A, const int N, const int LD, const float *Anew, const int N_Anew);
void copy_row_top(float *A, const int N, float *Anew);
void copy_row_bottom(float *A, const int N, const int LD, const int rows_per_proc, const float * Anew);

void handle_mpi_error(int proc_rank, int mpi_error);

#endif /* laplace_h */
