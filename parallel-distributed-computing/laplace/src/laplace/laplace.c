//
//  laplace.c
//  laplace
//
//  Created by Giuliano Aiello on 06/11/24.
//

#include "laplace.h"
#include "../../../common/mpi_proc/mpi_proc.h"
#include <mpi.h>

#define NEXT_TO_PREV 10
#define PREV_TO_NEXT 20

void laplace (float *A, float *Anew, float *daprev, float *danext, int N, int LD, int Niter){
    int proc_rank = -1, n_proc = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    int rows_per_proc = N / n_proc;

    int proc_rank_next = get_next_proc_rank(proc_rank);
    int proc_rank_prev = get_prev_proc_rank(proc_rank);

    int i;
    for (i = 0; i < Niter; ++i) {           // Niter can be seen as how much "heat" gets dissipated
        if (!(is_first_proc_rank(proc_rank))) {
            MPI_Send(A, N, MPI_FLOAT, proc_rank_prev, NEXT_TO_PREV, MPI_COMM_WORLD);
            MPI_Recv(daprev, N, MPI_FLOAT, proc_rank_prev, PREV_TO_NEXT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (!(is_last_proc_rank(proc_rank))) {
            MPI_Recv(danext, N, MPI_FLOAT, proc_rank_next, NEXT_TO_PREV, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&(A[rows_per_proc]), N, MPI_FLOAT, proc_rank_next, PREV_TO_NEXT, MPI_COMM_WORLD);
        }

        calculate_inner(A, N, LD, Anew, rows_per_proc);
        if (!(is_first_proc_rank(proc_rank))) {
            calculate_row_top(A, Anew, N, LD, daprev);
        }
        if (!(is_last_proc_rank(proc_rank))) {
            calculate_row_bottom(A, N, LD, danext, Anew, rows_per_proc);
        }

        copy_inner(A, Anew, rows_per_proc, N, LD);
        if (!(is_first_proc_rank(proc_rank))) {
            copy_row_top(A, Anew, rows_per_proc, N);
        }
        if (!(is_last_proc_rank(proc_rank))) {
            copy_row_bottom(A, Anew, rows_per_proc, N, LD);
        }
    }

}

/**
 @brief For every inner element of `Anew`, calculates its discrete Laplacian by taking into account elements of `A`.
 @param N_Anew a "logic" number of rows of `Anew` up to which (except first and last) the discrete Laplacian calculation will be applied. `Anew` physically has `N` rows.
 */
void calculate_inner(const float *A, const int N, const int LD, float *Anew, int N_Anew){
    int i, j;
    for (i = 1; i < N_Anew - 1; ++i) {
        for (j = 1; j < N - 1; ++j) {
            Anew[(i * LD) + j] = discrete_laplacian(A, i, j, LD);
        }
    }
}

/**
 @brief For every top-row element of `Anew`, calculates its discrete Laplacian by taking into account elements of `A` and `daprev` as the top row.
 */
void calculate_row_top(const float *A, float *Anew, const int N, const int LD, const float *daprev) {
    int i = 0, j;
    for (j = 1; j < N - 1; ++j) {
        Anew[(i * LD) + j] = discrete_laplacian_top(A, i, j, LD, daprev);
    }
}

/**
 @brief For every bottom-row element of `Anew`, calculates its discrete Laplacian by taking into account elements of `A` and `danext` as the bottom row.
 @param N_Anew the "logically" last row index of `Anew` which will be subjected to the discrete Laplacian calculation. `Anew` physically has `N` rows.
 */
void calculate_row_bottom(const float *A, const int N, const int LD, const float *danext, float *Anew, int N_Anew) {
    int i = N_Anew, j;
    for (j = 1; j < N - 1; ++j) {
        Anew[(i * LD) + j] = discrete_laplacian_bottom(A, i, j, LD, danext);
    }
}

/**
 * @param matrix from which the Laplacian calculation is based.
 * @param i index of the `matrix` element from which calculate the Laplacian.
 * @param j index of the `matrix` element from which calculate the Laplacian.
 */
float discrete_laplacian(const float *matrix, const int i, const int j, const int LD){
    float laplacian = 0.;

    laplacian = (matrix[i * LD + (j + 1)] +
                 matrix[i * LD + (j - 1)] +
                 matrix[(i + 1) * LD + j] +
                 matrix[(i - 1) * LD + j] )
                * 0.25;

    return laplacian;
}

/**
 * @param matrix from which the Laplacian calculation is based.
 * @param i index of the `matrix` element from which calculate the Laplacian.
 * @param j index of the `matrix` element from which calculate the Laplacian.
 * @param daprev that acts as the `matrix` top row in the Laplacian calculation.
 */
float discrete_laplacian_top(const float *matrix, const int i, const int j, const int LD, const float *daprev) {
    float laplacian = 0.;

    laplacian = (matrix[i * LD + (j + 1)] +
                 matrix[i * LD + (j - 1)] +
                 matrix[(i + 1) * LD + j] +
                 daprev[j])
                 * 0.25;

    return laplacian;
}

/**
 * @param matrix from which the Laplacian calculation is based.
 * @param i index of the `matrix` element from which calculate the Laplacian.
 * @param j index of the `matrix` element from which calculate the Laplacian.
 * @param danext that acts as the `matrix` bottom row in the Laplacian calculation.
 */
float discrete_laplacian_bottom(const float *matrix, const int i, const int j, const int LD, const float *danext) {
    float laplacian = 0.;

    laplacian = (matrix[i * LD + (j + 1)] +
                 matrix[i * LD + (j - 1)] +
                 matrix[(i - 1) * LD + j] +
                 danext[j])
                * 0.25;

    return laplacian;
}

/**
 @brief There is a mapping between element indexes of `A` and element indexes of `Anew` on which the Laplacian elements have been calculated.
        This function performs the "mapped" copy of the inner matrix of `Anew` into `A`.
 */
void copy_inner(float *A, float *Anew, int rows_per_proc, int N, const int LD) {
    int i, j;
    for (i = 1; i < rows_per_proc - 1; ++i) {
        for (j = 1; j < N - 1; ++j) {
            A[(i * LD) + j] = Anew[(i * LD) + j];
        }
    }
}

/**
 @brief There is a mapping between element indexes of `A` and element indexes of `Anew` on which the Laplacian elements have been calculated.
        This function performs the "mapped" copy of `Anew`'s top row into `A`.
 */
void copy_row_top(float *A, float *Anew, const int N, const int LD) {
    int i = 0, j;
    for (j = 1; j < N - 1; ++j) {
        A[(i * LD) + j] = Anew[(i * LD) + j];
    }
}

/**
 @brief There is a mapping between element indexes of `A` and element indexes of `Anew` on which the Laplacian elements have been calculated.
        This function performs the "mapped" copy of `Anew`'s bottom row into `A`.
 */
void copy_row_bottom(float *A, float * Anew, int rows_per_proc, const int N, const int LD) {
    int i = rows_per_proc, j;
    for (j = 1; j < N - 1; ++j) {
        A[(i * LD) + j] = Anew[(i * LD) + j];
    }
}
