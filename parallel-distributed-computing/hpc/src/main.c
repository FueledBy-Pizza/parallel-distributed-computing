//
//  main.c
//  hpc
//
//  Created by Giuliano Aiello on 16/11/24.
//

#include <stdio.h>
#include <stdlib.h>
#include "hpc/hpc.h"
#include "../../common/matrix/matrix.h"
#include "../../common/c_timer/c_timer.h"

int main(int argc, const char * argv[]) {
    int N = 256;
    int LD = N;

    double t0, t1, t2, t3;

    double *A = (double *) malloc(sizeof(double) * N * LD);
    double *B = (double *) malloc(sizeof(double) * N * LD);
    double *C = (double *) malloc(sizeof(double) * N * LD);

    init_matrix_sequentially_double(N, LD, A);
    init_matrix_sequentially_double(N, LD, B);
    init_matrix_sequentially_double(N, LD, C);

    /*print_matrix_double(N, LD, A);
    print_matrix_double(N, LD, B);
    print_matrix_double(N, LD, C);*/

    t0 = get_cur_time();
    matmatikj(C, A, B, N, LD);
    t1 = get_cur_time();

    /*printf("\nC after GeMM (ikj):\n");
    print_matrix_double(N, LD, C);*/
    printf("\nExecution time of GeMM (ikj) is %f(s)\n", t1 - t0);

    t2 = get_cur_time();
    matmatkji(C, A, B, N, LD);
    t3 = get_cur_time();

    /*printf("\nC after GeMM (kji):\n");
    print_matrix_double(N, LD, C);*/
    printf("\nExecution time of GeMM (kji) is %f(s)\n", t1 - t0);

    free(A);
    free(B);
    free(C);
}
