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

#ifndef mpi_proc_h
#define mpi_proc_h

#include <stdbool.h>

bool is_even_proc_rank(int proc_rank);
int get_next_proc_rank(int proc_rank);
int get_prev_proc_rank(int proc_rank);

#endif /* mpi_proc_h */
