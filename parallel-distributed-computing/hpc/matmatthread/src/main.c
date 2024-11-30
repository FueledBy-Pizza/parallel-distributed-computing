//
//  main.c
//  hpc-matmatthread
//
//  Created by Giuliano Aiello on 28/11/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matmatthread/matmatthread.h"
#include "../../../common/matrix/matrix.h"
#include "../../../common/c_timer/c_timer.h"

void print_cc(void);

int main(int argc, const char * argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <matrix_order> <dim_block> <numthread_row> <numthread_col>\n", argv[0]);
        return EXIT_FAILURE;
    }

    print_cc();

    const int N = atoi(argv[1]);
    const int dim_block = atoi(argv[2]);
    const int NTROW = atoi(argv[3]);
    const int NTCOL = atoi(argv[4]);
    const int LD = N;

    double *A = (double *) malloc(sizeof(double) * N * LD);
    double *B = (double *) malloc(sizeof(double) * N * LD);
    double *C = (double *) malloc(sizeof(double) * N * LD);

    const double Nflops = 2 * pow(N, 3);
    const double value = 1e9;

    double t0_t, t1_t = 0.;
    double exec_time_t = 0.;
    double Gflops_t = 0.;

    double t0_b, t1_b = 0.;
    double exec_time_b = 0.;
    double Gflops_b = 0.;

    const int N1 = N;
    const int N2 = N;
    const int N3 = N;
    const int LD_A = LD;
    const int LD_B = LD;
    const int LD_C = LD;

    init_matrix_sequentially_double(N1, LD_A, A);
    init_matrix_sequentially_double(N2, LD_B, B);
    init_matrix_sequentially_double(N1, LD_C, C);

    const int dimblock1 = dim_block;
    const int dimblock2 = dim_block;
    const int dimblock3 = dim_block;

    t0_b = get_cur_time();
    matmatblock(LD_A, LD_B, LD_C, A, B, C, N1, N2, N3, dimblock1, dimblock2, dimblock3);
    t1_b = get_cur_time();
    exec_time_b = t1_b - t0_b;
    Gflops_b = Nflops / exec_time_b / value;
    printf("\nMatmatblock (exploits GeMM ikj order). N: %d, DIM_BLOCK: %d\n", N, dim_block);
    printf("Runtime (s): %f, Gflops: %f\n", exec_time_b, Gflops_b);

    init_matrix_sequentially_double(N1, LD_C, C);

    t0_t = get_cur_time();
    matmatthread(LD_A, LD_B, LD_C, A, B, C, N1, N2, N3, dimblock1, dimblock2, dimblock3, NTROW, NTCOL);
    t1_t = get_cur_time();
    exec_time_t = t1_t - t0_t;
    Gflops_t = Nflops / exec_time_t / value;
    double speedup = exec_time_b / exec_time_t;
    double efficiency = speedup / (NTROW * NTCOL);
    printf("\nMatmatthread (exploits matmatblock). N: %d, DIM_BLOCK: %d, row thread(s): %d, col thread(s): %d\n", N, dim_block, NTROW, NTCOL);
    printf("Runtime (s): %f, Gflops: %f\n", exec_time_t, Gflops_t);
    printf("Speed-up: %f, Efficiency: %f\n", speedup, efficiency);

    free(A);
    free(B);
    free(C);
}

void print_cc(void) {
    #ifdef __clang__
        printf("\tCompiler: Clang\n");
        printf("\tVersion: %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
    #endif

    #ifdef __GNUC__
        // Ensure it's not Clang (Clang also defines __GNUC__)
    #ifndef __clang__
        printf("\tCompiler: GCC\n");
        printf("\tVersion: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #endif
    #endif

    #ifdef __OPTIMIZE__
        printf("\tOptimization: Enabled\n");
    #endif
}
