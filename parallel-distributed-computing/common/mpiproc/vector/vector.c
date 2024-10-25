//
//  vector.c
//  ring-sum
//
//  Created by Giuliano Aiello on 25/10/24.
//

#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

/**
 Pre-condition: only takes into account the communicator `MPI_COMM_WORLD` communicator.
 @param A vector to populate
 @param N dimension of A
 @return `A` populated by integers going from (`proc_rank * N`) to (`proc_rank * N`) + `N`
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
