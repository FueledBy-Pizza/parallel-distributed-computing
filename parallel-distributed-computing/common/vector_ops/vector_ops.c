//
//  vector_ops.c
//  ring-sum
//
//  Created by Giuliano Aiello on 25/10/24.
//

#include "vector_ops.h"

int array_sum(int *A, int length){
    int sum = 0;

    int i;
    for (i = 0; i < length; ++i) {
        sum += A[i];
    }

    return sum;
}
