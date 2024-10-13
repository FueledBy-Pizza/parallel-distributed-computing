//
//  maxsum.c
//  parallel-distributed-computing
//
//  Created by Giuliano Aiello on 11/10/24.
//

#include "maxsum.h"
#include <math.h>
#include <stdlib.h>
#include <omp.h>

double maxsum(int N, int LD, double *A, int NT){

    int start_row, end_row;
    int i, j;
    const int rows_per_thread = N / NT;
    double curr_sum = 0;
    double max_sum = curr_sum;

    double *MAX_SUMS = (double *) malloc(sizeof(double) * NT);
    double MAX_OVERALL = 0;

    omp_set_num_threads(NT);
    # pragma omp parallel private(start_row, end_row, i, j, curr_sum, max_sum)
    {
        int thread_id = omp_get_thread_num();
        start_row = thread_id * rows_per_thread;
        end_row = start_row + rows_per_thread;

        for (i = start_row; i < end_row; ++i) {
            curr_sum = 0;
            for (j = 0; j < LD; ++j) {
                curr_sum += sqrt(A[i * LD + j]);
            }
            if (curr_sum > max_sum)
                max_sum = curr_sum;
        }

        MAX_SUMS[thread_id] = max_sum;
    }

    int index = 0;
    for (index = 0; index < NT; ++index) {
        if (MAX_SUMS[index] > MAX_OVERALL) {
            MAX_OVERALL = MAX_SUMS[index];
        }
    }

    return MAX_OVERALL;

}
