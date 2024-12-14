//
//  matmatdist.c
//  hpc-matmatdist
//
//  Created by Giuliano Aiello on 04/12/24.
//

#include "matmatdist.h"
#include "../../../matmatthread/src/matmatthread/matmatthread.h"
#include "../../../matmatblock/src/matmatblock/matmatblock.h"
#include "../../../gemm/src/gemm/gemm.h"
#include "../../../../common/matrix/matrix.h"
#include "../../../../common/integer/integer.h"
#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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
