//
//  main.c
//  parallel-distributed-computing
//
//  Created by Giuliano Aiello on 11/10/24.
//

#include <stdio.h>
#include <omp.h>
#include "../common/c_timer.h"

int main(int argc, const char * argv[]) {
    double cur_time = get_cur_time();

    printf("Current time is %f\n", cur_time);

    return 0;
}
