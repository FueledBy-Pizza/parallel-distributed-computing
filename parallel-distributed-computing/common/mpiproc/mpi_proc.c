//
//  mpi_proc.c
//  ring-sum
//
//  Created by Giuliano Aiello on 25/10/24.
//

#include "mpi_proc.h"
#include "../integer/integer.h"

bool is_proc_rank_even(int proc_rank){
    return is_even(proc_rank);
}
