//
//  matmatdist.c
//  hpc-matmatdist
//
//  Created by Giuliano Aiello on 04/12/24.
//

#include "matmatdist.h"
#include "../../../../common/integer/integer.h"
#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/**
 @brief Copies `A` into `B`.
 */
void copy_matrix_double(double *A, int N_A, int LD_A, double *B, int N_B, int LD_B) {
    int N, LD, i, j;
    N = N_B;
    LD = LD_B;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < LD; ++j) {
            B[(i * LD) + j] = A[(i * LD_A) + j];
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
 @brief In blocks, performs a GeMM operation (`C`=`C`+`A``B`) in the ikj indexes order. "In blocks" means that a `C` block, an `A` block starting from the beginning of the row, a `B` block starting from the beginning of the column are needed (and then passed to matmatikj).
 */
void matmatblock(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3, int db1, int db2, int db3) {
    int ii, jj, kk;
    for (ii = 0; ii < N1; ii+=db1) {
        for (jj = 0; jj < N3; jj+=db3) {
            for (kk = 0; kk < N2; kk+=db2) {
                matmatikj(ldA, ldB, ldC, A + (ii * ldA) + kk, B + (kk * ldB) + jj, C + (ii * ldC) + jj, db1, db2, db3);
            }
        }
    }
}

/**
 @brief Performs a multithreaded matmatblock.
 */
void matmatthread(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3, int db1, int db2, int db3, int NTROW, int NTCOL) {
    const int n_threads = NTROW * NTCOL;
    const int submatrix_rows = N1 / NTROW;
    const int submatrix_cols = N3 / NTCOL;

    omp_set_num_threads(n_threads);
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();

        const int tid_i = tid / NTCOL;
        const int tid_j = tid % NTCOL;

        const int submatrix_start_i = tid_i * submatrix_rows;
        const int submatrix_start_j = tid_j * submatrix_cols;

        matmatblock(ldA, ldB, ldC, A + (submatrix_start_i * ldA), B + submatrix_start_j, C + (submatrix_start_i * ldC) + submatrix_start_j, N1 / NTROW, N2, N3 / NTCOL, db1, db2, db3);
    }
}

/**
 @brief This function is called by one or more processes to perform a process-distributed matmatthread.
 @param Gridcom is a two-dimensional process grid.
 @param A is part of a global matrix.
 @param B is part of a global matrix.
 @param C is part of a global matrix.
 */
void matmatdist(MPI_Comm Gridcom, int LDA, int LDB, int LDC, double *A, double *B, double *C, int N1, int N2, int N3, int db1, int db2, int db3, int NTROW, int NTCOL) {

    int ndims = 0;
    MPI_Cartdim_get(Gridcom, &ndims);
    if (ndims != 2) {
        fprintf(stderr, "Number of process grid dimensions (%d) is not equal to 2!\n", ndims);
        return;
    }

    int *dims    = malloc(sizeof(int) * ndims);
    int *periods = malloc(sizeof(int) * ndims);
    int *coords  = malloc(sizeof(int) * ndims);
    MPI_Cart_get(Gridcom, ndims, dims, periods, coords);

    int NProw = dims [0];
    int NPcol = dims [1];
    int mcm = lcm(NProw, NPcol);

    MPI_Comm comm_row;
    MPI_Comm comm_col;
    get_comm_row_col(Gridcom, &comm_row, &comm_col);

    int M1 = (N1 / NProw);
    int M2 = (N2 / mcm);
    int M3 = (N3 / NPcol);

    double *A_transmitted = malloc(sizeof(double) * M1 * M2);
    double *B_transmitted = malloc(sizeof(double) * M2 * M3);

    int k;
    for (k = 0; k < mcm; k++) {
        int rank_sender_to_row = k % NPcol;
        int rank_sender_to_col = k % NProw;

        copy_matrix_double(A, M1, LDA, A_transmitted, M1, M2);
        MPI_Bcast(A_transmitted, M1 * M2, MPI_DOUBLE, rank_sender_to_row, comm_row);

        copy_matrix_double(B, M2, LDB, B_transmitted, M2, M3);
        MPI_Bcast(B_transmitted, M2 * M3, MPI_DOUBLE, rank_sender_to_col, comm_col);

        matmatthread(M2, M3, LDC, A_transmitted, B_transmitted, C, M1, M2, M3, db1, db2, db3, NTROW, NTCOL);
    }

    free(A_transmitted);
    free(B_transmitted);

    MPI_Comm_free(&comm_row);
    MPI_Comm_free(&comm_col);

    free(dims);
    free(periods);
    free(coords);

}

void get_comm_row_col(MPI_Comm comm_grid, MPI_Comm *comm_row, MPI_Comm *comm_col) {
    int remain_dims_row[2] = {0, 1};
    int remain_dims_col[2] = {1, 0};

    MPI_Cart_sub(comm_grid, remain_dims_row, comm_row);
    MPI_Cart_sub(comm_grid, remain_dims_col, comm_col);
}
