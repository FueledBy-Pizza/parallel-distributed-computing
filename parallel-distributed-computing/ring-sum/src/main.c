//
//  main.c
//  ring-sum
//
//  Created by Giuliano Aiello on 22/10/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../../common/mpi_proc/mpi_proc_vector/mpi_proc_vector.h"
#include "ring-sum/ring-sum.h"

int main(int argc, const char * argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <vector_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    //int *A = NULL;
    //int vector_size = atoi(argv[1]);

    MPI_Init(&argc, (char ***) &argv);

    int proc_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    printf("Hello\n");
    //populate_vector_by_proc_rank(A, vector_size);

    //int sum = somma(A, vector_size);

    //printf("Hello from process %d, total sum is %d", proc_rank);

    MPI_Finalize();

    return 0;
}
