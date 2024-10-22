//
//  main.c
//  parallel-distributed-computing
//
//  Created by Giuliano Aiello on 11/10/24.
//

#include <stdio.h>
#include <stdlib.h>
#include "../common/c_timer.h"
#include "src/matrix_ops/matrix_ops.h"
#include "src/maxsum/maxsum.h"

int main(int argc, const char * argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <matrix_order>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int NT;
    int N = atoi(argv[1]);
    int LD = N;
    double MAX, *A = NULL;
    double t1, t2, exec_time, speedup, efficiency, one_thread_exec_time = 0.0;

    init_matrix_randomly(&N, &LD, &A);

    //print_matrix(N, LD, A);

    for (NT = 1; NT <= 8; NT = NT * 2) {
        printf("===============\n");
        printf("THREAD ID = %d\n", NT);

        t1 = get_cur_time();
        MAX = maxsum(N, LD, A, NT);
        t2 = get_cur_time();

        if (NT == 1)
            one_thread_exec_time = t2-t1;

        exec_time = t2 - t1;
        speedup = one_thread_exec_time / exec_time;
        efficiency = speedup / NT;

        printf("The maximum sum of the square roots from any row (with N = %d) is %f\n", N,  MAX);
        printf("Execution time is %e, speedup is %f, efficiency is %f\n", exec_time, speedup, efficiency);
    }

    return 0;
}
