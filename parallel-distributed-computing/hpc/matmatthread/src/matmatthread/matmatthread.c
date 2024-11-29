//
//  matmatthread.c
//  hpc-matmatthread
//
//  Created by Giuliano Aiello on 28/11/24.
//

#include "matmatthread.h"
#include "../../../matmatblock/src/matmatblock/matmatblock.h"
#include <omp.h>
#include <stdio.h>

void matmatthread(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3, int db1, int db2, int db3, int NTROW, int NTCOL) {
    const int n_threads = NTROW * NTCOL;

    omp_set_num_threads(n_threads);
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();

        const int i_tid = tid / N3;
        const int j_tid = tid % N3;

        matmatblock(ldA, ldB, ldC, A + (i_tid * ldA), B + j_tid, C + (i_tid * ldC) + j_tid, N1 / NTROW, N2, N3 / NTCOL, db1, db2, db3);
    }
}
