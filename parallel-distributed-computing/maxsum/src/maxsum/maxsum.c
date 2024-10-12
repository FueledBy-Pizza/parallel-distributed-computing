//
//  maxsum.c
//  parallel-distributed-computing
//
//  Created by Giuliano Aiello on 11/10/24.
//

#include "maxsum.h"
#include <math.h>

double maxsum(int N, int LD, double *A, int NT){

    int n_threads = 4;
    int start_row = log2(NT) * (N/n_threads);
    int end_row = start_row + 200;

    int i, j;
    double max_sum = 0;
    double curr_sum;
    for (i = start_row; i < end_row; ++i) {
        curr_sum = 0;
        for (j = 0; j < LD; ++j) {
            curr_sum += sqrt(A[i * LD + j]);
        }
        if (curr_sum > max_sum)
            max_sum = curr_sum;
    }

    return max_sum;
}
