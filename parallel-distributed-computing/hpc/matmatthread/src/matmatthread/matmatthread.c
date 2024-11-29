//
//  matmatthread.c
//  hpc-matmatthread
//
//  Created by Giuliano Aiello on 28/11/24.
//

#include "matmatthread.h"
#include <omp.h>
#include <stdio.h>

void matmatthread(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3, int db1, int db2, int db3, int NTROW, int NTCOL) {
    const int n_threads = NTROW * NTCOL;
    const int N = N1 * N3;

    omp_set_num_threads(n_threads);
    #pragma omp parallel
    {
        const int thread_id = omp_get_thread_num();
        const int thread_i = 0;
        const int thread_j = 0;
        printf("Hello from thread %d\n", omp_get_thread_num());
    }
}
