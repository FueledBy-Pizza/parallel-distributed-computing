//
//  matmatthread.c
//  hpc-matmatthread
//
//  Created by Giuliano Aiello on 28/11/24.
//

#include "matmatthread.h"
#include "../../../matmatblock/src/matmatblock/matmatblock.h"
#include <omp.h>

/**
 @brief Performs a multithreaded matmatblock.
 */
void matmatthread(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3, int db1, int db2, int db3, int NTROW, int NTCOL) {
    const int n_threads = NTROW * NTCOL;
    const int submatrix_rows = N1 / NTROW;
    const int submatrix_cols = N3 / NTCOL;

    omp_set_num_threads(n_threads);
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();

        const int tid_i = tid / NTCOL;
        const int tid_j = tid % NTCOL;

        const int submatrix_start_i = tid_i * submatrix_rows;
        const int submatrix_start_j = tid_j * submatrix_cols;

        matmatblock(ldA, ldB, ldC, A + (submatrix_start_i * ldA), B + submatrix_start_j, C + (submatrix_start_i * ldC) + submatrix_start_j, N1 / NTROW, N2, N3 / NTCOL, db1, db2, db3);
    }
}
