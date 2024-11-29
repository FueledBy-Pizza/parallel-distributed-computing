#!/bin/bash

if [ ! -x ./build/hpc-matmatthread.out ]; then
    echo "Executable not found. Exiting."
    exit 1
fi

if [ -f config.sh ]; then
    source config.sh
else
    N=4096
    DIM_BLOCK=256
    NTROW=4
    NTCOL=4
fi

./build/hpc-matmatthread.out $N $DIM_BLOCK $NTROW $NTCOL
