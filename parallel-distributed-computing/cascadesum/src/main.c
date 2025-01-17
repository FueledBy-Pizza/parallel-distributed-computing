//
//  main.c
//  cascadesum
//
//  Created by Giuliano Aiello on 16/01/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "cascadesum/cascadesum.h"
#include "../../common/vector/vector.h"
#include "../../common/integer/integer.h"
#include "../../common/c_timer/c_timer.h"

# define PROC_ROOT 0

void print_cc(void);

int main(int argc, const char * argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <vector_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int vector_size = atoi(argv[1]);

    const char *mpi_proc_env = getenv("OMPI_COMM_WORLD_SIZE");
    const int mpi_proc = atoi(mpi_proc_env);

    if ((vector_size % mpi_proc) != 0) {
        fprintf(stderr, "vector_size % mpi_proc must be zero!\n");
        return EXIT_FAILURE;
    }

    print_cc();

    int *global_vector = NULL;
    global_vector = (int *) malloc(sizeof(int) * vector_size);
    populate_vector_sequentially(global_vector, vector_size);

    double t1 = get_cur_time();
    int vec_sum = vector_sum(global_vector, vector_size);
    double t2 = get_cur_time();
    double t_singleprocess = t2 - t1;

    MPI_Init(&argc, (char ***) &argv);

    int proc_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == PROC_ROOT) {
        printf("VECTOR_SIZE is %d, the vector will contain integers from 0 to %d.\n", vector_size, vector_size - 1);
    }

    int n_proc = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    int n_items_per_proc = vector_size / n_proc;
    int local_vector[n_items_per_proc];
    MPI_Scatter(global_vector, n_items_per_proc, MPI_INT, local_vector, n_items_per_proc, MPI_INT, PROC_ROOT, MPI_COMM_WORLD);

    t1 = get_cur_time();
    vec_sum = cascadesum(local_vector, n_items_per_proc);
    t2 = get_cur_time();
    double t_multiprocess = t2 - t1;

    double speedup = t_singleprocess / t_multiprocess;
    double efficiency = speedup / n_proc;

    if (proc_rank == PROC_ROOT) {
        printf("Hello from root process, vector sum is %d.\n", vec_sum);
        printf("\nSingle-process execution time is %e, multi-process execution time is %e.\nSpeedup: %f\nEfficiency: %f\n", t_singleprocess, t_multiprocess, speedup, efficiency);
        printf("\nTheoretical ideal speedup should have been %e.\n", t_singleprocess / n_proc);
    }

    MPI_Finalize();

    free(global_vector);

    return 0;
}

void print_cc(void) {
    #ifdef __clang__
        printf("\tCompiler: Clang\n");
        printf("\tVersion: %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
    #endif

    #ifdef __GNUC__
        // Ensure it's not Clang (Clang also defines __GNUC__)
    #ifndef __clang__
        printf("\tCompiler: GCC\n");
        printf("\tVersion: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #endif
    #endif

    #ifdef __OPTIMIZE__
        printf("\tOptimization: Enabled\n");
    #endif
}
