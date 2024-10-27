/******************************************************************************
 * Library Name: `vector`
 * Overview: this library provides functions for basic vector operations.
 * Version: 1.0.0
 * Author: Giuliano Aiello
 * Created: 2024-10-25
 * Updated: 2024-10-27
 * License: MIT
 *
 */

#include "vector.h"

int vector_sum(int *A, int length){
    int sum = 0;

    int i;
    for (i = 0; i < length; ++i) {
        sum += A[i];
    }

    return sum;
}
