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
#include "../../../common/matrix/matrix.h"
#include "../../../common/c_timer/c_timer.h"
#include "../../../common/integer/integer.h"

#define PROC_GRID_DIMENSIONS 2
#define NO_REORDER 0

void split_matrices_to_processes(double *A, double *B, double *C, int M1, int M2, int M3, int N2, int N3, int *coords, int *dims, int ld, int mcm) {
    int i, j;

    for (i = 0; i < M1; i++){
        for (j = 0; j < M2; j++){
            A[(i * ld) + j] = coords[0] * dims[1] * (N2 / mcm) + coords[1] * (N2 / mcm) + (i * N2) + j;
        }
    }
    for (i = 0; i < M2; i++){
        for (j = 0; j < M3; j++){
            B[(i * ld) + j] = 10 + coords[0] * dims[1] * N3 + coords[1] * (N3 / dims[1]) + (i * N3) + j;
        }
    }
    for (i = 0; i < M1; i++){
        for (j = 0; j < M3; j++){
            C[(i * ld) + j] = 0.0;
        }
    }
}

void print_local_matrix(double *A, int N, int M, int ld, int proc_rank, char mat) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            printf("proc_rank %d. %c(%d, %d)->  %f \n", proc_rank, mat, i, j, A[(i * ld) + j]);
        }
        printf("\n");
    }
    printf("-------------------------------------\n");
}

int main(int argc, char * argv[]) {

    int NProw = atoi(argv[1]);
    int NPcol = atoi(argv[2]);

    MPI_Init(&argc, &argv);

    int comm_world_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_world_size);
    if (comm_world_size != (NProw * NPcol)) {
        fprintf(stderr, "Number of process launched (%d) is not equal to NProw X NPcol (%d X %d)!\n", comm_world_size, NProw, NPcol);
        return EXIT_FAILURE;
    }

    MPI_Comm comm_grid;
    int dims[2]    = {NProw, NPcol};
    int periods[2] = {1, 1};
    MPI_Cart_create(MPI_COMM_WORLD, PROC_GRID_DIMENSIONS, dims, periods, NO_REORDER, &comm_grid);

    int ld = 6144;
    double *A = (double *) malloc(sizeof(double) * ld * ld);
    double *B = (double *) malloc(sizeof(double) * ld * ld);
    double *C = (double *) malloc(sizeof(double) * ld * ld);

    int proc_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD,&proc_rank);
    int coords[2];
    MPI_Cart_coords(comm_grid, proc_rank, 2, coords);

    int N1 = 2;
    int N2 = 4;
    int N3 = 4;
    int mcm = lcm(NProw, NPcol);

    // The three submatrices dimensions.
    int M1 = N1 / NProw;
    int M2 = N2 / mcm;
    int M3 = N3 / NPcol;
    split_matrices_to_processes(A, B, C, M1, M2, M3, N2, N3, coords, dims, ld, mcm);

    int TROW = 1;
    int TCOL = 1;
    matmatdist(comm_grid, ld, ld, ld, A, B, C, N1, N2, N3, 1, 1, 1, TROW, TCOL);

    print_local_matrix(A, M1, M2, ld, proc_rank, 'A');
    print_local_matrix(B, M2, M3, ld, proc_rank, 'B');
    print_local_matrix(C, M1, M3, ld, proc_rank, 'C');

    init_matrix_randomly_double(ld, ld, A);
    init_matrix_randomly_double(ld, ld, B);
    init_matrix_randomly_double(ld, ld, C);

    int step = 2048;
    int N;
    double value = 1.e9;
    if(proc_rank == 0)
        printf("              %d x %d processes.\n     N       T       time       Gflops\n", NProw, NPcol);
    for (N = step; N <= (step * 3); N += step) {
        double t0, t1, exec_time;
        double Nflops = 2 * pow(N, 3);
        double Gflops = 0.;
        int dim_block = 256;

        TROW = 1; TCOL = 1;
        MPI_Barrier(MPI_COMM_WORLD);
        t0 = get_cur_time();
        matmatdist(comm_grid, ld, ld, ld, A, B, C, N, N, N, dim_block, dim_block, dim_block, TROW, TCOL);
        t1 = get_cur_time();
        exec_time = t1 - t0;
        Gflops = Nflops / exec_time / value;
        if (proc_rank == 0)
            printf("   %4d   %4d   %e  %f \n", N, TROW * TCOL, exec_time,  Gflops);

        TROW = 2; TCOL = 1;
        MPI_Barrier(MPI_COMM_WORLD);
        t0 = get_cur_time();
        matmatdist(comm_grid, ld, ld, ld, A, B, C, N, N, N, dim_block, dim_block, dim_block, TROW, TCOL);
        t1 = get_cur_time();
        exec_time = t1 - t0;
        Gflops = Nflops / exec_time / value;
        if (proc_rank == 0)
            printf("   %4d   %4d   %e  %f \n", N, TROW * TCOL, exec_time, Gflops);

        TROW = 2; TCOL = 2;
        MPI_Barrier(MPI_COMM_WORLD);
        t0 = get_cur_time();
        matmatdist(comm_grid, ld, ld, ld, A, B, C, N, N, N, dim_block, dim_block, dim_block, TROW, TCOL);
        t1 = get_cur_time();
        exec_time = t1 - t0;
        Gflops = Nflops / exec_time / value;
        if (proc_rank == 0)
            printf("   %4d   %4d   %e  %f \n", N, TROW * TCOL, exec_time, Gflops);

        TROW = 4; TCOL = 2;
        MPI_Barrier(MPI_COMM_WORLD);
        t0 = get_cur_time();
        matmatdist(comm_grid, ld, ld, ld, A, B, C, N, N, N, dim_block, dim_block, dim_block, TROW, TCOL);
        t1 = get_cur_time();
        exec_time = t1 - t0;
        Gflops = Nflops / exec_time / value;
        if (proc_rank == 0)
            printf("   %4d   %4d   %e  %f \n\n", N, TROW * TCOL, exec_time, Gflops);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    free(A);
    free(B);
    free(C);

    MPI_Finalize();

}
