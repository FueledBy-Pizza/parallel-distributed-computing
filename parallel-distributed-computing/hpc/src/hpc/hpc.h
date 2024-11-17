//
//  hpc.h
//  hpc
//
//  Created by Giuliano Aiello on 17/11/24.
//

#ifndef hpc_h
#define hpc_h

void gemm_ikj(double *C, const double *A, const double *B, const int N, const int LD);
void gemm_kji(double *C, const double *A, const double *B, const int N, const int LD);

#endif /* hpc_h */
