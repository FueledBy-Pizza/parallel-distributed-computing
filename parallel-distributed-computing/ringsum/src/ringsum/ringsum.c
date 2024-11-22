//
//  ringsum.c
//  ringsum
//
//  Created by Giuliano Aiello on 23/10/24.
//

#include "ringsum.h"
#include "../../../common/vector/vector.h"
#include "../../../common/mpi_proc/mpi_proc.h"
#include <mpi.h>
#include <stdbool.h>

int ringsum(int *A, int N){

    int proc_rank, num_proc = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    int partial_sum_incoming = 0;
    int partial_sum_outgoing = calculate_partial_sum(A, N);
    int total_sum = partial_sum_outgoing;

    int proc_rank_receiver = get_next_proc_rank(proc_rank);
    int proc_rank_sender = get_prev_proc_rank(proc_rank);

    const int TAG = 0;

    int i;
    for (i = 0; i < num_proc - 1; ++i) {
        if (is_even_proc_rank(proc_rank)) {
            MPI_Send(&partial_sum_outgoing, 1, MPI_INT, proc_rank_receiver, TAG, MPI_COMM_WORLD);
            MPI_Recv(&partial_sum_incoming, 1, MPI_INT, proc_rank_sender, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(&partial_sum_incoming, 1, MPI_INT, proc_rank_sender, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&partial_sum_outgoing, 1, MPI_INT, proc_rank_receiver, TAG, MPI_COMM_WORLD);
        }

        total_sum += partial_sum_incoming;
        partial_sum_outgoing = partial_sum_incoming;
    }

    return total_sum;
}

int calculate_partial_sum(int *A, int N){
    return vector_sum(A, N);
}
