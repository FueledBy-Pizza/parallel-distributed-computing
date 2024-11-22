#!/bin/bash

if [ ! -x ./build/ringsum.out ]; then
    echo "Executable not found. Exiting."
    exit 1
fi

if [ -f config.sh ]; then
    source config.sh
else
    MPI_PROCS=4
    VECTOR_SIZE=4
fi

mpiexec -np $MPI_PROCS ./build/ringsum.out $VECTOR_SIZE
