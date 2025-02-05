//
//  main.c
//  matmatblock
//
//  Created by Giuliano Aiello on 22/11/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matmatblock/matmatblock.h"
#include "../../../common/matrix/matrix.h"
#include "../../../common/c_timer/c_timer.h"

void print_cc(void);

int main(int argc, const char * argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <matrix_order> <dim_block>\n", argv[0]);
        return EXIT_FAILURE;
    }

    print_cc();

    const int N = atoi(argv[1]);
    const int dim_block = atoi(argv[2]);
    const int LD = N;

    double *A = (double *) malloc(sizeof(double) * N * LD);
    double *B = (double *) malloc(sizeof(double) * N * LD);
    double *C = (double *) malloc(sizeof(double) * N * LD);

    const double Nflops = 2 * pow(N, 3);
    const double value = 1e9;
    double t0, t1 = 0.;
    double exec_time = 0.;
    double Gflops = 0.;

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

    t0 = get_cur_time();
    matmatblock(LD_A, LD_B, LD_C, A, B, C, N1, N2, N3, dimblock1, dimblock2, dimblock3);
    t1 = get_cur_time();
    exec_time = t1 - t0;
    Gflops = Nflops / exec_time / value;
    printf("\nMatmatblock (exploits GeMM ikj order). N: %d, DIM_BLOCK: %d\n", N, dim_block);
    printf("Runtime (s): %f, Gflops: %f\n", exec_time, Gflops);

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
