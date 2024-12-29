//
//  main.c
//  laplace
//
//  Created by Giuliano Aiello on 04/11/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include "../../common/c_timer/c_timer.h"
#include "laplace/laplace.h"
#include "../../common/matrix/matrix.h"

int main(int argc, const char * argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <matrix_order> <N_iterations>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int N = atoi(argv[1]);
    int Niter = atoi(argv[2]);

    int nproc, myid;
    int i, j, ifirst, LD;
    float *A, *Anew, *daprev, *danext;
    double t1, t2;

    MPI_Init(&argc, (char ***) &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    sleep(1);

    LD = N;
    A = (float *) malloc(N * N * sizeof(float));
    Anew = (float *) malloc(N * N * sizeof(float));
    daprev = (float *) malloc(N * sizeof(float));
    danext = (float *) malloc(N * sizeof(float));

    for (i = 0; i < (N / nproc); i++){                      // Initialize local matrix to 0.
        for (j = 0; j < N; j++){
            A[(i * LD) + j] = 0.;
        }
    }

    if (myid == 0){                                         // First row of the matrix pf process ranked 0 from 0 to N-1.
        for (j = 0; j < N; j++){
            A[(0 * LD) + j] = j;
        }
    }

    if (myid == nproc - 1){                                 // Last row of the matrix of process ranked N-1 from N-1 to 0.
        for (j = 0; j < N; j++){
            A[((N / nproc) - 1) * LD + j] = N - 1 - j;
        }
    }

    ifirst = myid * (N / nproc);
    for (i = 0; i < (N / nproc); i++){
        A[(i * LD) + 0] = ifirst + i;                       // Left border from ifirst a ilast-1 in every process.
        A[(i * LD) + (N - 1)] = N - 1 - A[(i * LD) + 0];    // A[i][0] + A[i][N-1] = 0 in every process.
    }

    if (myid ==0)
        printf("N = %d\nIterations = %d\n", N, Niter);

    t1 = get_cur_time();
    laplace(A, Anew, daprev, danext, N, LD, Niter);
    t2 = get_cur_time();

    if (myid == 0)
        printf("\nFunction %s addressed the problem in %f(s) running time with %d processes.\n", "laplace", t2 - t1, nproc);

    if (myid==0)
        printf("\nFirst row %d --> %f      %f", myid, A[(1 * LD) + 1], A[(1 * LD) + 398]);
    if (myid==3)
        printf("\nCenter %d    --> %f      %f", myid, A[(49 * LD) + 199], A[(49 * LD) + 200]);
    if (myid==4)
        printf("\nCenter %d    --> %f      %f", myid, A[(0 * LD) + 199], A[(0 * LD) + 200]);
    if (myid==7)
        printf("\nLast row %d  --> %f    %f\n", myid, A[(48 * LD) + 1], A[(48 * LD) + 398]);

    free(A);
    free(Anew);
    free(daprev);
    free(danext);

    MPI_Finalize();

    return 0;
}
