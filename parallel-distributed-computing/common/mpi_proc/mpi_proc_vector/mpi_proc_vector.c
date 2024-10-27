/******************************************************************************
 * Library Name: `mpi_proc_vector`
 *
 * Overview:
 * Read the `mpi_proc` main component before reading this.
 * Provides functions for basic vector operations within the scope of MPI processes.
 *
 * Version: 1.0.0
 * Author: Giuliano Aiello
 * Created: 2024-10-25
 * Updated: 2024-10-27
 * License: MIT
 *
 */

#include "mpi_proc_vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

/**
 @param A pointer to an integer array, will be populated by integers going from (`proc_rank * N`) to (`proc_rank * N`) + `N`
 @param N dimension of A
 */
void populate_vector_by_proc_rank(int *A, int N){
    A = (int *) malloc(sizeof(int) * N);

    int proc_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == -1)
        fprintf(stderr, "Error: could not retrieve process rank.\n");

    int i;
    for (i = 0; i < N; ++i) {
        A[i] = proc_rank * N + i;
    }
}
