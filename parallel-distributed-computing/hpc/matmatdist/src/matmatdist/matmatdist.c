//
//  matmatdist.c
//  hpc-matmatdist
//
//  Created by Giuliano Aiello on 04/12/24.
//

#include "matmatdist.h"
#include "../../../matmatthread/src/matmatthread/matmatthread.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/**
 @brief This function is called by one or more processes to perform a process-distributed matmatthread.
 @param comm_grid is a two-dimensional process grid.
 @param A is part of a global matrix.
 @param B is part of a global matrix.
 @param C is part of a global matrix.
 */
void matmatdist(MPI_Comm comm_grid, int LDA, int LDB, int LDC, double *A, double *B, double *C, int N1, int N2, int N3, int db1, int db2, int db3, int NTROW, int NTCOL) {

    int ndims = 0;
    MPI_Cartdim_get(comm_grid, &ndims);
    if (ndims != 2) {
        fprintf(stderr, "Number of process grid dimensions (%d) is not equal to 2!\n", ndims);
        return;
    }

    int *dims    = malloc(sizeof(int) * ndims);
    int *periods = malloc(sizeof(int) * ndims);
    int *coords  = malloc(sizeof(int) * ndims);
    MPI_Cart_get(comm_grid, ndims, dims, periods, coords);

    int NProw = dims [0];
    int NPcol = dims [1];
    int mcm = lcm(NProw, NPcol);

    MPI_Comm comm_row;
    MPI_Comm comm_col;
    get_comm_row_col(comm_grid, &comm_row, &comm_col);

    int k;
    for (k = 0; k < mcm; k++) {
        int sender_to_row = k % NPcol;
        int sender_to_col = k % NProw;

        MPI_Bcast(A, N1 / NProw, MPI_DOUBLE, sender_to_row, comm_row);
        MPI_Bcast(B, N2 / NPcol, MPI_DOUBLE, sender_to_col, comm_col);

        matmatthread(LDA, LDB, LDC, A, B, C, N1, N2, N3, db1, db2, db3, NTROW, NTCOL);
    }

    free(dims);
    free(periods);
    free(coords);

    MPI_Comm_free(&comm_row);
    MPI_Comm_free(&comm_col);
}

void get_comm_row_col(MPI_Comm comm_grid, MPI_Comm *comm_row, MPI_Comm *comm_col) {
    int remain_dims_row[2] = {0, 1};
    int remain_dims_col[2] = {1, 0};

    MPI_Cart_sub(comm_grid, remain_dims_row, comm_row);
    MPI_Cart_sub(comm_grid, remain_dims_col, comm_col);
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}
