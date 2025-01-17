//
//  main.c
//  cascadesum
//
//  Created by Giuliano Aiello on 16/01/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "cascadesum/cascadesum.h"
#include "../../common/vector/vector.h"
#include "../../common/integer/integer.h"

# define PROC_ROOT 0

int main(int argc, const char * argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <vector_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int vector_size = atoi(argv[1]);

    const char *mpi_proc_env = getenv("OMPI_COMM_WORLD_SIZE");
    const int mpi_proc = atoi(mpi_proc_env);

    if ((vector_size % mpi_proc) != 0) {
        fprintf(stderr, "vector_size % mpi_proc must be zero!\n");
        return EXIT_FAILURE;
    }

    int *global_vector = NULL;
    global_vector = (int *) malloc(sizeof(int) * vector_size);
    populate_vector_sequentially(global_vector, vector_size);

    MPI_Init(&argc, (char ***) &argv);

    int proc_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == PROC_ROOT) {
        printf("VECTOR_SIZE is %d, the vector will contain integers from 0 to %d.\n", vector_size, vector_size - 1);
    }

    int n_proc = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    int n_items_per_proc = vector_size / n_proc;
    int local_vector[n_items_per_proc];
    MPI_Scatter(global_vector, n_items_per_proc, MPI_INT, local_vector, n_items_per_proc, MPI_INT, PROC_ROOT, MPI_COMM_WORLD);

    int vector_sum = cascadesum(local_vector, n_items_per_proc);
    if (proc_rank == PROC_ROOT) {
        printf("\nHello from root process, vector sum is %d.\n", vector_sum);
    }

    MPI_Finalize();

    free(global_vector);

    return 0;
}
