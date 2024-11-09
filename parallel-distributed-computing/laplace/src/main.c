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

    LD = N + 100;
    A = (float*)malloc(LD*LD*sizeof(float));
    Anew = (float*)malloc(LD*LD*sizeof(float));
    daprev = (float*)malloc(LD*sizeof(float));
    danext = (float*)malloc(LD*sizeof(float));

    for (i = 0; i< N/nproc; i++){              // tutta la matrice locale = 0
        for (j = 0; j< N; j++){
            A[i*LD+j] = 0.;
        }
    }
    if (myid == 0){
        for (j = 0; j < N; j++){
            A[0*LD+j] = j;                // prima riga matrice del proc id=0  da 0 a 399
        }
    }

    if (myid == nproc-1){
        for (j = 0; j < N; j++){
            A[(N/nproc-1)*LD+j] = N - 1 - j;      // ultima riga matrice del proc id=nproc-1 da 399 a 0
        }
    }

    ifirst = myid * N/nproc;
    for (i = 0; i < N/nproc; i++){
        A[i*LD+0] = ifirst + i;             // bordo sinistro da ifirst a ilast-1 in ogni proc
        A[i*LD+N-1] = N - 1 - A[i*LD+0];      // A[i][0] + A[i][N-1] = 0 sempre
    }

    if (myid ==0) printf("N = %d\nIterations = %d\n\n", N, Niter);

    t1 = get_cur_time();
    laplace (A, Anew, daprev, danext, N, LD, Niter);
    t2 = get_cur_time();

    if (myid == 0) printf("%f running time with %d processes\n\n", t2-t1, nproc);

    sleep(1);

    if (myid==0)
        printf("prima  %d -->   %f  %f  \n", myid, A[1*LD+1], A[1*LD+398]);
    if (myid==3)
        printf("centro %d -->   %f  %f  \n", myid, A[49*LD+199], A[49*LD+200]);
    if (myid==4)
        printf("centro %d -->   %f  %f  \n", myid, A[00*LD+199], A[00*LD+200]);
    if (myid==7)
        printf("ultima %d -->   %f  %f  \n", myid, A[48*LD+1], A[48*LD+398]);

    MPI_Finalize();

}
