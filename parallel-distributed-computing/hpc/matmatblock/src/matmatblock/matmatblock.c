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
    // doc: draw matrix multiplication by clearly writing the scope of N1, N2, N3, indexes etc. (and the @brief explanation, too -pdf 13, pp.2-)
    // doc: matmatblock (theoretically) enhances performance only because of cache accesses, so there is no optimal calculation in choosing the best dim_block as a function of N, it only depends on cache, so, and on machine.
    // warning! "performance do not collapse" on slides only refers to cluster environment: we have proved that on macOS with Clang performance lightly collapse in matmatblock when increasing N, while even less collapse in matmatikj when increasing N
    int ii, jj, kk;
    for (ii = 0; ii < N1; ii+=db1) {
        for (jj = 0; jj < N3; jj+=db3) {
            for (kk = 0; kk < N2; kk+=db2) {
                matmatikj(ldA, ldB, ldC, A + (ii * ldA) + kk, B + (kk * ldB) + jj, C + (ii * ldC) + jj, db1, db2, db3);
            }
        }
    }
}
