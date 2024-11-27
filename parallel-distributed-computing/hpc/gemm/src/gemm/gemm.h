//
//  gemm.h
//  gemm
//
//  Created by Giuliano Aiello on 17/11/24.
//

#ifndef gemm_h
#define gemm_h

void matmatijk(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);
void matmatkji(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);
void matmatikj(int ldA, int ldB, int ldC, double *A, double *B, double *C, int N1, int N2, int N3);
void matmatjik(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);
void matmatkij(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);
void matmatjki(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);

#endif /* gemm_h */
