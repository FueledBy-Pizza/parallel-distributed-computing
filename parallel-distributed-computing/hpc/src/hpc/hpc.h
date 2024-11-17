//
//  hpc.h
//  hpc
//
//  Created by Giuliano Aiello on 17/11/24.
//

#ifndef hpc_h
#define hpc_h

void matmatijk(double *C, const double *A, const double *B, const int N, const int LD);
void matmatkji(double *C, const double *A, const double *B, const int N, const int LD);
void matmatikj(double *C, const double *A, const double *B, const int N, const int LD);
void matmatjik(double *C, const double *A, const double *B, const int N, const int LD);
void matmatkij(double *C, const double *A, const double *B, const int N, const int LD);
void matmatjki(double *C, const double *A, const double *B, const int N, const int LD);

#endif /* hpc_h */
