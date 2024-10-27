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

    int proc_rank = -1, num_proc = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    int partial_sum_received = 0;
    int partial_sum_calculated = calculate_partial_sum(A, N);
    int total_sum = partial_sum_calculated;

    const int TAG = 0;
    int proc_rank_receiver = get_next_proc_rank(proc_rank);
    int proc_rank_sender = get_prev_proc_rank(proc_rank);

    int i;
    for (i = 0; i < (num_proc - 1); ++i) {
        if (is_proc_rank_even(proc_rank)) {
            MPI_Send(&partial_sum_calculated, 1, MPI_INT, proc_rank_receiver, TAG, MPI_COMM_WORLD);
            MPI_Recv(&partial_sum_received, 1, MPI_INT, proc_rank_sender, TAG, MPI_COMM_WORLD, NULL);

            total_sum += partial_sum_received;

            proc_rank_receiver = get_next_proc_rank(proc_rank_receiver);
            proc_rank_sender = get_prev_proc_rank(proc_rank_sender);
        } else {
            MPI_Recv(&partial_sum_received, 1, MPI_INT, proc_rank_sender, TAG, MPI_COMM_WORLD, NULL);
            MPI_Send(&partial_sum_calculated, 1, MPI_INT, proc_rank_receiver, TAG, MPI_COMM_WORLD);

            total_sum += partial_sum_received;

            proc_rank_receiver = get_next_proc_rank(proc_rank_receiver);
            proc_rank_sender = get_prev_proc_rank(proc_rank_sender);
        }
    }

    return total_sum;
}

int calculate_partial_sum(int *A, int N){
    return vector_sum(A, N);
}
