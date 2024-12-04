//
//  matmatdist.h
//  hpc-matmatdist
//
//  Created by Giuliano Aiello on 04/12/24.
//

#ifndef matmatdist_h
#define matmatdist_h

#include <mpi.h>

void matmatdist(MPI_Comm comm, int LDA, int LDB, int LDC, double *A, double *B, double *C, int N1, int N2, int N3, int db1, int db2, int db3, int NTROW, int NTCOL);

#endif /* matmatdist_h */
