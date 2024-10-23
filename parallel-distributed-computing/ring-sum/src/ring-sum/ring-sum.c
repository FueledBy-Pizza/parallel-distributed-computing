//
//  ring-sum.c
//  ring-sum
//
//  Created by Giuliano Aiello on 23/10/24.
//

#include "ring-sum.h"
#include <stdlib.h>
#include <mpi.h>

int somma(int *A, int N){

    A = (int *) malloc(sizeof(int) * N);
    int proc_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    populate_vector_by_procrank(A, N, proc_rank);

    return 0;
}

void populate_vector_by_procrank(int *A, int N, int proc_rank){
    int i;
    for (i = 0; i < N; ++i) {
        A[i] = proc_rank * N + i;
    }
}
