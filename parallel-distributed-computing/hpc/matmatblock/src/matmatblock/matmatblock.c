//
//  matmatblock.c
//  matmatblock
//
//  Created by Giuliano Aiello on 22/11/24.
//

#include "matmatblock.h"
#include "../../../gemm/src/gemm/gemm.h"
#include <stdio.h>

void matmatblock(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3, int dbA, int dbB, int dbC) {
    // doc: draw matrix multiplication by clearly writing the scope of N1, N2, N3, indexes etc. (pdf 13 - pp.2)
    int ii, jj, kk;
    for (ii = 0; ii < N1; ii+=dbA) {
        for (jj = 0; jj < N3; jj+=dbC) {
            for (kk = 0; kk < N2; kk+=dbB) {
                matmatikj(ldA, ldB, ldC, &(A[(ii * ldA) + kk]), &(B[(kk * ldB) + jj]), &(C[(ii * ldC) + jj]), dbA, dbB, dbC);
            }
        }
    }
}
