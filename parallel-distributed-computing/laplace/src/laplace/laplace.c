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

    int i;
    for (i = 0; i < Niter; ++i) {                       // Niter can be seen as how much "heat" gets dissipated.
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

    }
}

void laplace_nb (float *A, float *Anew, float *daprev, float *danext, int N, int LD, int Niter){
    int proc_rank = -1, n_proc = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    int rows_per_proc = N / n_proc;
    int last_row_index = (rows_per_proc - 1) * LD;

    int proc_rank_prev = get_prev_proc_rank(proc_rank);
    int proc_rank_next = get_next_proc_rank(proc_rank);

    int i;
    for (i = 0; i < Niter; ++i) {                       // Niter can be seen as how much "heat" gets dissipated.
        MPI_Request request_daprev, request_danext, request_sendlast, request_sendfirst;
        MPI_Status status_danext, status_daprev, status_sendfirst, status_sendlast;

        if (!(is_first_proc_rank(proc_rank))) {
            MPI_Isend(&(A[0]), N, MPI_FLOAT, proc_rank_prev, NEXT_TO_PREV, MPI_COMM_WORLD, &request_sendfirst);
            MPI_Irecv(daprev, N, MPI_FLOAT, proc_rank_prev, PREV_TO_NEXT, MPI_COMM_WORLD, &request_daprev);
        }
        if (!(is_last_proc_rank(proc_rank))) {
            MPI_Irecv(danext, N, MPI_FLOAT, proc_rank_next, NEXT_TO_PREV, MPI_COMM_WORLD, &request_danext);
            MPI_Isend(&(A[last_row_index]), N, MPI_FLOAT, proc_rank_next, PREV_TO_NEXT, MPI_COMM_WORLD, &request_sendlast);
        }

        calculate_inner(A, N, LD, Anew, rows_per_proc);
        // Before reading a variable that is going to be recv, it must be ensured it's already recv.
        if (!(is_first_proc_rank(proc_rank))){
            if (MPI_Wait(&request_daprev, &status_daprev) == MPI_SUCCESS)
                calculate_row_top(A, N, LD, Anew, daprev);
            else
                handle_mpi_error(proc_rank, status_daprev.MPI_ERROR);
        }
        if (!(is_last_proc_rank(proc_rank))){
            if (MPI_Wait(&request_danext, &status_danext) == MPI_SUCCESS)
                calculate_row_bottom(A, N, LD, rows_per_proc, Anew, danext);
            else
                handle_mpi_error(proc_rank, status_danext.MPI_ERROR);
        }

        copy_inner(A, N, LD, Anew, rows_per_proc);
        // Before writing a variable that is going to be sent, it must be ensured it's already sent.
        if (!(is_first_proc_rank(proc_rank))){
            if (MPI_Wait(&request_sendfirst, &status_sendfirst) == MPI_SUCCESS)
                copy_row_top(A, N, Anew);
            else
                handle_mpi_error(proc_rank, status_sendfirst.MPI_ERROR);
        }
        if (!(is_last_proc_rank(proc_rank))){
            if (MPI_Wait(&request_sendlast, &status_sendlast) == MPI_SUCCESS)
                copy_row_bottom(A, N, LD, rows_per_proc, Anew);
            else
                handle_mpi_error(proc_rank, status_sendlast.MPI_ERROR);
        }
    }
}

/**
 @brief For every inner element of `Anew`, calculates its discrete Laplacian by taking into account elements of `A`.
 @param N_Anew a "logic" number of rows of `Anew` up to which (except first and last) the discrete Laplacian calculation will be applied. `Anew` physically has `N` rows.
 */
void calculate_inner(const float *A, const int N, const int LD, float *Anew, const int N_Anew){
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
 * @brief Calculates the discrete Laplacian of the (`i`,`j`)-th element of `A`.
 * @param A matrix from which the Laplacian calculation is based.
 * @param i index of the `A` element from which calculate the Laplacian.
 * @param j index of the `A` element from which calculate the Laplacian.
 */
float discrete_laplacian(const float *A, const int i, const int j, const int LD){
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

void handle_mpi_error(int proc_rank, int mpi_error) {
    char mpi_error_string[100];
    int mpi_error_string_length;

    MPI_Error_string(mpi_error, mpi_error_string, &mpi_error_string_length);
    fprintf(stderr, "Process ranked %d failed with MPI error: %s\n", proc_rank, mpi_error_string);
}
