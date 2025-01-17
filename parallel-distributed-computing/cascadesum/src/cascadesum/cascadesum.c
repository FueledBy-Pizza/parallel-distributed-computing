//
//  cascadesum.c
//  cascadesum
//
//  Created by Giuliano Aiello on 16/01/25.
//

#include "cascadesum.h"
#include "../../../common/vector/vector.h"
#include <mpi.h>
#include <math.h>

# define LEFT_TO_RIGHT 10
# define RIGHT_TO_LEFT 20

int cascadesum(int *A, int length) {

    int sum_tot = vector_sum(A, length);
    int sum_par = 0;

    int proc_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int q = proc_rank;

    int n_procs = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

    int offset = 1;

    int i;
    int n_steps = log2(n_procs);
    for (i = 0; i < n_steps; ++i) {

        int group = q % 2;
        if (group == 0) {
            MPI_Send(&sum_tot, 1, MPI_INT, proc_rank + offset, LEFT_TO_RIGHT, MPI_COMM_WORLD);
            MPI_Recv(&sum_par, 1, MPI_INT, proc_rank + offset, RIGHT_TO_LEFT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(&sum_par, 1, MPI_INT, proc_rank - offset, LEFT_TO_RIGHT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&sum_tot, 1, MPI_INT, proc_rank - offset, RIGHT_TO_LEFT, MPI_COMM_WORLD);
        }
        sum_tot += sum_par;

        q /= 2;
        offset *= 2;
    }

    return sum_tot;
}
