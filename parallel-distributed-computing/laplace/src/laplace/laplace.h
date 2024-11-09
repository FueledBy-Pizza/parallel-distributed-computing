//
//  laplace.h
//  laplace
//
//  Created by Giuliano Aiello on 06/11/24.
//

#ifndef laplace_h
#define laplace_h

void laplace (float *A, float *Anew, float *daprev, float *danext, int N, int LD, int Niter);

void calculate_inner(const float *A, const int N, const int LD, float *Anew, int N_Anew);
void calculate_row_top(const float *A, float *Anew, const int N, const int LD, const float *daprev);
void calculate_row_bottom(const float *A, const int N, const int LD, const float *danext, float *Anew, int N_Anew);

float discrete_laplacian(const float *A, const int i, const int j, const int LD);
float discrete_laplacian_top(const float *matrix, const int i, const int j, const int LD, const float *daprev);
float discrete_laplacian_bottom(const float *matrix, const int i, const int j, const int LD, const float *danext);

void copy_inner(float *A, float *Anew, int rows_per_proc, int N, const int LD);
void copy_row_top(float *A, float *Anew, const int N, const int LD);
void copy_row_bottom(float *A, float * Anew, int rows_per_proc, const int N, const int LD);

#endif /* laplace_h */
