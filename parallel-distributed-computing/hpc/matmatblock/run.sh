#!/bin/bash

if [ ! -x ./build/hpc-matmatblock.out ]; then
    echo "Executable not found. Exiting."
    exit 1
fi

if [ -f config.sh ]; then
    source config.sh
else
    N=4096
    DIM_BLOCK=256
fi

./build/hpc-matmatblock.out $N $DIM_BLOCK
