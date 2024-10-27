//
//  ring-sum.c
//  ring-sum
//
//  Created by Giuliano Aiello on 23/10/24.
//

#include "ring-sum.h"
#include "../../../common/vector/vector.h"
#include "../../../common/mpi_proc/mpi_proc.h"
#include <mpi.h>
#include <stdbool.h>

int somma(int *A, int N){

    int proc_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    int calculated_partial_sum = partial_sum(A, N);

    if (is_proc_rank_even(proc_rank)) {
        
    } else {
        
    }

    return calculated_partial_sum;
}

int partial_sum(int *A, int N){
    return vector_sum(A, N);
}
