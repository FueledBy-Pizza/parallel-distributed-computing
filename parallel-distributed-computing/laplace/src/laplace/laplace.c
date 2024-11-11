//
//  laplace.c
//  laplace
//
//  Created by Giuliano Aiello on 06/11/24.
//

#include "laplace.h"
#include "../../../common/mpi_proc/mpi_proc.h"
#include "../../../common/matrix/matrix.h"
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#define NEXT_TO_PREV 10
#define PREV_TO_NEXT 20

void laplace (float *A, float *Anew, float *daprev, float *danext, int N, int LD, int Niter){
    int proc_rank = -1, n_proc = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    int rows_per_proc = N / n_proc;
    int last_row_index = (rows_per_proc - 1) * LD;

    int proc_rank_prev = get_prev_proc_rank(proc_rank);
    int proc_rank_next = get_next_proc_rank(proc_rank);

    /*if(proc_rank == 1)
        sleep(1);

    print_matrix(N / n_proc, LD, A);
    if (proc_rank == 1) {
        printf("\n");
    }*/

    int i;
    for (i = 0; i < Niter; ++i) {           // Niter can be seen as how much "heat" gets dissipated
        if (!(is_first_proc_rank(proc_rank))) {
            MPI_Send(&(A[0]), N, MPI_FLOAT, proc_rank_prev, NEXT_TO_PREV, MPI_COMM_WORLD);
            MPI_Recv(daprev, N, MPI_FLOAT, proc_rank_prev, PREV_TO_NEXT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (!(is_last_proc_rank(proc_rank))) {
            MPI_Recv(danext, N, MPI_FLOAT, proc_rank_next, NEXT_TO_PREV, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&(A[last_row_index]), N, MPI_FLOAT, proc_rank_next, PREV_TO_NEXT, MPI_COMM_WORLD);
        }

        calculate_inner(A, N, LD, Anew, rows_per_proc);
        if (!(is_first_proc_rank(proc_rank))){
            calculate_row_top(A, N, LD, Anew, daprev);
        }
        if (!(is_last_proc_rank(proc_rank))){
            calculate_row_bottom(A, N, LD, rows_per_proc, Anew, danext);
        }

        copy_inner(A, N, LD, Anew, rows_per_proc);
        if (!(is_first_proc_rank(proc_rank))){
            copy_row_top(A, N, Anew);
        }
        if (!(is_last_proc_rank(proc_rank))){
            copy_row_bottom(A, N, LD, rows_per_proc, Anew);
        }

        /*if(proc_rank == 1)
            sleep(1);

        print_matrix(N / n_proc, LD, A);
        if (proc_rank == 1) {
            printf("\n");
        }*/
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
 @brief For every inner top-row element of `Anew`, calculates its discrete Laplacian by taking into account elements of `A` and `daprev` as the top row.
 */
void calculate_row_top(const float *A, const int N, const int LD, float *Anew, const float *daprev) {
    int j;
    for (j = 1; j < N - 1; ++j) {
        Anew[j] = discrete_laplacian_top(A, j, LD, daprev);
    }
}

/**
 @brief For every inner bottom-row element of `Anew`, calculates its discrete Laplacian by taking into account elements of `A` and `danext` as the bottom row.
 */
void calculate_row_bottom(const float *A, const int N, const int LD, const int rows_per_proc, float *Anew, const float *danext) {
    int last_row = (rows_per_proc - 1) * LD;
    int j;
    for (j = 1; j < N - 1; ++j) {
        Anew[last_row + j] = discrete_laplacian_bottom(A, rows_per_proc - 1, j, LD, danext);
    }
}

/**
 * @param A matrix from which the Laplacian calculation is based.
 * @param i index of the `A` element from which calculate the Laplacian.
 * @param j index of the `A` element from which calculate the Laplacian.
 */
float discrete_laplacian(const float *A, const int i, const int j, int LD){
    float laplacian = 0.;

    laplacian = (A[(i * LD) + (j + 1)] +
                 A[(i * LD) + (j - 1)] +
                 A[((i + 1) * LD) + j] +
                 A[((i - 1) * LD) + j] )
                * 0.25;

    return laplacian;
}

/**
 * @brief Calculates the discrete Laplacian of the `j`-th top-row element of `A`.
 * @param A matrix from which the Laplacian calculation is based.
 * @param j index of the `A` top-row element from which calculate the Laplacian.
 * @param daprev acts as the `A` top row in the Laplacian calculation.
 */
float discrete_laplacian_top(const float *A, const int j, const int LD, const float *daprev) {
    float laplacian = 0.;

    laplacian = (A[j + 1] +
                 A[j - 1] +
                 A[LD + j] +
                 daprev[j])
                 * 0.25;

    return laplacian;
}

/**
 * @brief Calculates the discrete Laplacian of the `j`-th bottom-row element of `A`.
 * @param A matrix from which the Laplacian calculation is based.
 * @param j index of the `A` element from which calculate the Laplacian.
 * @param danext acts as the `A` bottom row in the Laplacian calculation.
 */
float discrete_laplacian_bottom(const float *A, const int i, const int j, const int LD, const float *danext) {
    float laplacian = 0.;

    laplacian = (A[(i * LD) + (j + 1)] +
                 A[(i * LD) + (j - 1)] +
                 A[((i - 1) * LD) + j] +
                 danext[j])
                * 0.25;
    int myid = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    /*if (myid == 0) {
        if (j == 1) {
            printf("i: %d, j: %d, LD: %d\n", i, j, LD);
            printf("A[i + (j + 1)] = %f\n", A[(i * LD) + (j + 1)]);
            printf("A[i + (j - 1)] = %f\n", A[(i * LD) + (j - 1)]);
            printf("A[((i - 1) * LD) + j] = %f\n", A[((i - 1) * LD) + j]);
            printf("danext[j] = %f\n", danext[j]);
        }
    }*/

    return laplacian;
}

/**
 @brief Performs a copy of `Anew`'s inner matrix into `A`'s inner matrix.
 */
void copy_inner(float *A, const int N, const int LD, const float *Anew, const int N_Anew) {
    int i, j;
    for (i = 1; i < N_Anew - 1; ++i) {
        int row = i * LD;
        for (j = 1; j < N - 1; ++j) {
            A[row + j] = Anew[row + j];
        }
    }
}

/**
 @brief Performs a copy of `Anew`'s top-row into `A`'s top-row.
 */
void copy_row_top(float *A, const int N, float *Anew) {
    int j;
    for (j = 1; j < N - 1; ++j) {
        A[j] = Anew[j];
    }
}

/**
 @brief Performs a copy of `Anew`'s bottom-row into `A`'s bottom-row.
 */
void copy_row_bottom(float *A, const int N, const int LD, const int rows_per_proc, const float * Anew) {
    int last_row = (rows_per_proc - 1) * LD, j;
    for (j = 1; j < N - 1; ++j) {
        A[last_row + j] = Anew[last_row + j];
    }
}
