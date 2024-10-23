//
//  main.c
//  ring-sum
//
//  Created by Giuliano Aiello on 22/10/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "src/ring-sum/ring-sum.h"

int main(int argc, const char * argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <vector_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int *A;
    int vector_size = atoi(argv[1]);

    MPI_Init(&argc, (char ***) &argv);

    int sum = somma(A, vector_size);

    MPI_Finalize();

    return 0;
}
