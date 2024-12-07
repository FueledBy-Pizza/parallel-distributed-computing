//
//  matmatdist.c
//  hpc-matmatdist
//
//  Created by Giuliano Aiello on 04/12/24.
//

#include "matmatdist.h"
#include <mpi.h>
#include <stdio.h>

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

    int coords[2];
    get_comm_grid_coords(comm_grid, ndims, coords);

    MPI_Comm comm_row;
    MPI_Comm comm_col;
    get_comm_row_col(comm_grid, &comm_row, &comm_col);

    /*int rank_grid = -1;
    MPI_Comm_rank(comm_grid, &rank_grid);
    printf("grid_rank %d is (%d, %d)\n", rank_grid, coords[0], coords[1]);*/

    MPI_Comm_free(&comm_row);
    MPI_Comm_free(&comm_col);
}

void get_comm_grid_coords(MPI_Comm comm, int ndims, int *coords) {
    int grid_rank = -1;
    MPI_Comm_rank(comm, &grid_rank);
    MPI_Cart_coords(comm, grid_rank, ndims, coords);
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
