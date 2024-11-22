//
//  main.c
//  ringsum
//
//  Created by Giuliano Aiello on 22/10/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../../common/mpi_proc/mpi_proc_vector/mpi_proc_vector.h"
#include "ringsum/ringsum.h"

int main(int argc, const char * argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <vector_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int *A = NULL;
    int vector_size = atoi(argv[1]);

    MPI_Init(&argc, (char ***) &argv);

    int proc_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    A = (int *) malloc(sizeof(int) * vector_size);
    populate_vector_by_proc_rank(A, vector_size);

    int total_sum = ringsum(A, vector_size);

    printf("Hello from process ranked %d, total sum is %d.\n", proc_rank, total_sum);

    MPI_Finalize();

    return 0;
}
