//
//  hpc.h
//  hpc
//
//  Created by Giuliano Aiello on 17/11/24.
//

#ifndef hpc_h
#define hpc_h

void matmatijk(int ldA, int ldB, int ldC, double *A, double*B, double *C, int N1, int N2, int N3);
void matmatkji(double *C, const double *A, const double *B, const int N, const int LD);
void matmatikj(double *C, const double *A, const double *B, const int N, const int LD);
void matmatjik(double *C, const double *A, const double *B, const int N, const int LD);
void matmatkij(double *C, const double *A, const double *B, const int N, const int LD);
void matmatjki(double *C, const double *A, const double *B, const int N, const int LD);

#endif /* hpc_h */
