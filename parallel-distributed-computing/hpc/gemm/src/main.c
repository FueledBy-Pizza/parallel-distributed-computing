//
//  main.c
//  gemm
//
//  Created by Giuliano Aiello on 16/11/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "gemm/gemm.h"
#include "../../../common/matrix/matrix.h"
#include "../../../common/c_timer/c_timer.h"

void print_cc(void);

int main(int argc, const char * argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <matrix_order>\n", argv[0]);
        return EXIT_FAILURE;
    }

    print_cc();

    int N = atoi(argv[1]);
    int LD = N;

    double *A = (double *) malloc(sizeof(double) * N * LD);
    double *B = (double *) malloc(sizeof(double) * N * LD);
    double *C = (double *) malloc(sizeof(double) * N * LD);

    const double Nflops = 2 * pow(N, 3);
    const double value = 1e9;
    double t0, t1 = 0.;
    double exec_time = 0.;
    double Gflops = 0.;

    init_matrix_sequentially_double(N, LD, A);
    init_matrix_sequentially_double(N, LD, B);
    init_matrix_sequentially_double(N, LD, C);

    t0 = get_cur_time();
    matmatijk(LD, LD, LD, A, B, C, N, N, N);
    t1 = get_cur_time();
    exec_time = t1 - t0;
    Gflops = Nflops / exec_time / value;
    printf("\nGeMM ijk order. N: %d\n", N);
    printf("Runtime (s): %f, Gflops: %f\n", exec_time, Gflops);

    sleep(1);

    t0 = get_cur_time();
    matmatkji(LD, LD, LD, A, B, C, N, N, N);
    t1 = get_cur_time();
    exec_time = t1 - t0;
    Gflops = Nflops / exec_time / value;
    printf("\nGeMM kji order. N: %d\n", N);
    printf("Runtime (s): %f, Gflops: %f\n", exec_time, Gflops);

    sleep(1);

    t0 = get_cur_time();
    matmatikj(LD, LD, LD, A, B, C, N, N, N);
    t1 = get_cur_time();
    exec_time = t1 - t0;
    Gflops = Nflops / exec_time / value;
    printf("\nGeMM ikj order. N: %d\n", N);
    printf("Runtime (s): %f, Gflops: %f\n", exec_time, Gflops);

    sleep(1);

    t0 = get_cur_time();
    matmatjik(LD, LD, LD, A, B, C, N, N, N);
    t1 = get_cur_time();
    exec_time = t1 - t0;
    Gflops = Nflops / exec_time / value;
    printf("\nGeMM jik order. N: %d\n", N);
    printf("Runtime (s): %f, Gflops: %f\n", exec_time, Gflops);

    sleep(1);

    t0 = get_cur_time();
    matmatkij(LD, LD, LD, A, B, C, N, N, N);
    t1 = get_cur_time();
    exec_time = t1 - t0;
    Gflops = Nflops / exec_time / value;
    printf("\nGeMM kij order. N: %d\n", N);
    printf("Runtime (s): %f, Gflops: %f\n", exec_time, Gflops);

    sleep(1);

    t0 = get_cur_time();
    matmatjki(LD, LD, LD, A, B, C, N, N, N);
    t1 = get_cur_time();
    exec_time = t1 - t0;
    Gflops = Nflops / exec_time / value;
    printf("\nGeMM jki order. N: %d\n", N);
    printf("Runtime (s): %f, Gflops: %f\n\n", exec_time, Gflops);

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
