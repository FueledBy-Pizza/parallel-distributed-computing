//
//  hpc.h
//  hpc
//
//  Created by Giuliano Aiello on 17/11/24.
//

#ifndef hpc_h
#define hpc_h

void matmatijk(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);
void matmatkji(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);
void matmatikj(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);
void matmatjik(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);
void matmatkij(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);
void matmatjki(int ldA, int ldB, int ldC, double *C, double *A, double *B, int N1, int N2, int N3);

#endif /* hpc_h */
