#!/bin/bash

if [ ! -x ./build/laplace.out ]; then
    echo "Executable not found. Exiting."
    exit 1
fi

if [ -f config.sh ]; then
    source config.sh
else
    N=400
    N_ITER=8000
fi

mpiexec -np $MPI_PROCS ./build/laplace.out $N $N_ITER
