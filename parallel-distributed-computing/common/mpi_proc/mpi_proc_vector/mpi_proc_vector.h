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

#ifndef mpi_proc_vector_h
#define mpi_proc_vector_h

void populate_vector_by_proc_rank(int *A, int N);

#endif /* mpi_proc_vector_h */
