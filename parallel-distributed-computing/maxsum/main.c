//
//  main.c
//  parallel-distributed-computing
//
//  Created by Giuliano Aiello on 11/10/24.
//

#include <stdio.h>
#include <omp.h>
#include "../common/c_timer.h"
#include "src/maxsum/maxsum.h"

void init_matrix_randomly(int *N, int *LD, double **A);
void print_matrix(int N, int LD, double *A);

int main(int argc, const char * argv[]) {
    
    int NT;
    int N = 800;
    int LD = N;
    double MAX, *A = NULL;
    double t1, t2, save;
    
    init_matrix_randomly(&N, &LD, &A);

    print_matrix(N, LD, A);
    
    return 0;
}
