//
//  matmatblock.c
//  matmatblock
//
//  Created by Giuliano Aiello on 22/11/24.
//

#include "matmatblock.h"
#include "../../../gemm/src/gemm/gemm.h"
#include <stdio.h>

/**
 @brief In blocks, performs a GeMM operation (`C`=`C`+`A``B`) in the ikj indexes order. "In blocks" means that a `C` block, an `A` block starting from the beginning of the row, a `B` block starting from the beginning of the column are needed (and then passed to matmatikj).
 */
void matmatblock(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3, int db1, int db2, int db3) {
    int ii, jj, kk;
    for (ii = 0; ii < N1; ii+=db1) {
        for (jj = 0; jj < N2; jj+=db2) {
            for (kk = 0; kk < N3; kk+=db3) {
                matmatikj(ldA, ldB, ldC, A + (ii * ldA) + jj, B + (jj * ldB) + kk, C + (ii * ldC) + kk, db1, db2, db3);
            }
        }
    }
}
