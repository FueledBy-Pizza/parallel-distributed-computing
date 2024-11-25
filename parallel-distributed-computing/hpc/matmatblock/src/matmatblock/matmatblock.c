//
//  matmatblock.c
//  matmatblock
//
//  Created by Giuliano Aiello on 22/11/24.
//

#include "matmatblock.h"
#include "../../../gemm/src/gemm/gemm.h"

void matmatblock(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3, int dbA, int dbB, int dbC) {
    int ii, kk, jj;
    for (ii = 0; ii < N1; ii+=dbA) {
        for (kk = 0; kk < N2; kk+=dbB) {
            for (jj = 0; jj < N3; jj+=dbC) {
                matmatikj(ldA, ldB, ldC, &(C[ii]), &(A[jj]), &(B[kk]), dbA, dbB, dbC);
            }
        }
    }
}
