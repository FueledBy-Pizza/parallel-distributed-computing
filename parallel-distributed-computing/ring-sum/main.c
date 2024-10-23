//
//  main.c
//  ring-sum
//
//  Created by Giuliano Aiello on 22/10/24.
//

#include <stdio.h>
#include <mpi.h>

int main(int argc, const char * argv[]) {
    MPI_Init(&argc, (char ***) &argv);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    printf("hello from process %d of %d total processes\n", world_rank, world_size);
    MPI_Finalize();
    
    return 0;
}
