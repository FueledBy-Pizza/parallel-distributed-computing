/******************************************************************************
 * Library Name: `integer`
 * Overview: this library provides functions for basic integer operations.
 * Version: 1.0.0
 * Author: Giuliano Aiello
 * Created: 2024-10-25
 * Updated: 2024-10-27
 * License: MIT
 *
 */

#include "integer.h"
#include <stdbool.h>

bool is_even(int x){
    return (x%2 == 0);
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

    int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}
