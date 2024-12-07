//
//  main.c
//  hpc-matmatdist
//
//  Created by Giuliano Aiello on 04/12/24.
//

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include "matmatdist/matmatdist.h"
#include "../../../common/c_timer/c_timer.h"

int main(int argc, char * argv[]) {
    // usare questo programma chiamante per fare test di correttezza per matmatdist
    // SOLO CON GLIGLIE DI PROCESSI (NPROW , NPCOL) = (1,1) e (2,2)

    int i, j, N1, N2, N3, ld, mcm ;
    int dims[2], period[2], coord[2], TROW, TCOL, rank, size;
    int X, Y, Q, R;
    double *A, *B, *C, *D;
    double time1, time2, Ndouble;
    MPI_Comm GridCom;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    //
    // qua viene definita la griglia di processi
    // ATTENZIONE: il prodotto dims[0]*dims[1] deve essere uguale al
    // numero di processi lanciati da mpirun nel file.pbs
    //
    dims[0] = 2;
    dims[1] = 2;
    int NProw = dims[0];
    int NPcol = dims[1];
    period[0] = 1;
    period[1] = 1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, period, 0, &GridCom);

    //
    // allocazione dello spazio per i test
    //
    ld = 6144;
    A=(double*)malloc(sizeof(double)*ld*ld);
    B=(double*)malloc(sizeof(double)*ld*ld);
    C=(double*)malloc(sizeof(double)*ld*ld);
    D=(double*)malloc(sizeof(double)*ld*ld);


    // ==================================================
    // test di correttezza risultati. Verificare solo per griglie di processi (1,1) e (2,2)
    // ==================================================

    N1 = 2;
    N2 = 4;
    N3 = 4;
    TROW = 1; TCOL =1 ;

    MPI_Cart_coords(GridCom, rank, 2, coord);

    // il calcolo del mcm serve solo per la stampa del risultato del test di correttezza
    X = dims[0]; Y = dims[1];
    while(Y != 0){
        Q = X/Y;
        R = X - Q*Y;
        X = Y;
        Y = R;
    }
    mcm = (dims[0] * dims[1]) / X;

    //
    // definizione delle matrici di input
    //
    for (i=0; i<N1/NProw; i++){
        for (j=0; j<N2/mcm; j++){
            A[i*ld+j] = coord[0]*dims[1]*N2/mcm + coord[1]*N2/mcm + i*N2+j;
        }
    }
    for (i=0; i<N2/mcm; i++){
        for (j=0; j<N3/NPcol; j++){
            B[i*ld+j] = 10 + coord[0]*dims[1]*N3 + coord[1]*N3/dims[1] + i*N3+j;
        }
    }
    for (i=0; i<N1/NProw; i++){
        for (j=0; j<N3/NPcol; j++){
            C[i*ld+j] = 0.0;
        }
    }

    matmatdist(GridCom, ld, ld, ld, A, B, C, N1, N2, N3, 1, 1, 1, TROW, TCOL);

    //
    // stampa delle matrici A, B e C
    //
    /*
    for (i=0; i<N1/NProw; i++){
        for (j=0; j<N2/mcm; j++){
            printf("MAT A id %d (%d,%d)->  %f \n", rank, i, j, A[i*ld+j]);
        }
        printf("\n");
    }
    printf("------------------\n");

    for (i=0; i<N2/mcm; i++){
        for (j=0; j<N3/NPcol; j++){
            printf("MAT B id %d (%d,%d)->  %f \n", rank, i, j, B[i*ld+j]);
        }
        printf("\n");
    }
    printf("------------------\n");
    for (i=0; i<N1/NProw; i++){
        for (j=0; j<N3/NPcol; j++){
            printf("MAT C id %d (%d,%d)->  %f \n", rank, i, j, C[i*ld+j]);
        }
        printf("\n");
    }

    // ==================================================
    // test di efficienza
    // ==================================================

    srand(0);
    for(i=0; i<ld; i++){
        for(j=0; j<ld; j++){
            *(A+i*ld+j) = (float)rand()/RAND_MAX;
            *(B+i*ld+j) = (float)rand()/RAND_MAX;
            *(C+i*ld+j) = (float)rand()/RAND_MAX;
            *(D+i*ld+j) =  *(C+i*ld+j) ;
        }
    }


    if(rank==0) printf("               N         time       Gflops\n");
    for (N1 = 2048; N1 <= 2048*3; N1 = N1+2048){
        Ndouble = N1;

        TROW = 1; TCOL =1 ; // test con 1 thread per processo
        MPI_Barrier(MPI_COMM_WORLD);
        time1=get_cur_time();
        matmatdist(GridCom, ld, ld, ld, A, B, C, N1, N1, N1, 256, 256, 256, TROW, TCOL);
        time2=get_cur_time()-time1;
        printf(" proc = %d:   %4d   %4d   %e  %f \n",rank, N1, TROW*TCOL, time2,  2*Ndouble*Ndouble*Ndouble/time2/1.e9);

        TROW = 2; TCOL =1 ; // test con 2 thread per processo
        MPI_Barrier(MPI_COMM_WORLD);
        time1=get_cur_time();
        matmatdist(GridCom, ld, ld, ld, A, B, C, N1, N1, N1, 256, 256, 256, TROW, TCOL);
        time2=get_cur_time()-time1;
        printf(" proc = %d:   %4d   %4d   %e  %f \n",rank, N1, TROW*TCOL, time2,  2*Ndouble*Ndouble*Ndouble/time2/1.e9);

        TROW = 2; TCOL =2 ; // test con 4 thread per processo
        MPI_Barrier(MPI_COMM_WORLD);
        time1=get_cur_time();
        matmatdist(GridCom, ld, ld, ld, A, B, C, N1, N1, N1, 256, 256, 256, TROW, TCOL);
        time2=get_cur_time()-time1;
        printf(" proc = %d:   %4d   %4d   %e  %f \n",rank, N1, TROW*TCOL, time2,  2*Ndouble*Ndouble*Ndouble/time2/1.e9);

        TROW = 4; TCOL =2 ; // test con 8 thread per processo
        MPI_Barrier(MPI_COMM_WORLD);
        time1=get_cur_time();
        matmatdist(GridCom, ld, ld, ld, A, B, C, N1, N1, N1, 256, 256, 256, TROW, TCOL);
        time2=get_cur_time()-time1;
        printf(" proc = %d:   %4d   %4d   %e  %f \n",rank, N1, TROW*TCOL, time2,  2*Ndouble*Ndouble*Ndouble/time2/1.e9);

    }*/

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

}
