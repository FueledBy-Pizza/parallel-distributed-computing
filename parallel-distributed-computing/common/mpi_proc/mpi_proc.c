/******************************************************************************
 * Library Name: `mpi_proc`
 *
 * Overview:
 * This library provides functions for basic MPI processes operations.
 * All functions in this library, as well as any subcomponents (e.g., `mpi_proc_vector`), communicate exclusively through the `MPI_COMM_WORLD` communicator.
 *
 * Version: 1.0.0
 * Author: Giuliano Aiello
 * Created: 2024-10-25
 * Updated: 2024-10-27
 * License: MIT
 *
 */

#include "mpi_proc.h"
#include "../integer/integer.h"
#include <mpi.h>

bool is_proc_rank_even(int proc_rank){
    return is_even(proc_rank);
}

int get_next_proc_rank(void){
    int proc_rank, num_procs, next_proc_rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    next_proc_rank = (proc_rank + num_procs + 1) % num_procs;

    return next_proc_rank;
}
