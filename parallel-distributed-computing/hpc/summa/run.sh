#!/bin/bash

if [ ! -x ./build/hpc-summa.out ]; then
    echo "Executable not found. Exiting."
    exit 1
fi

if [ -f config.sh ]; then
    source config.sh
else
    N=4096
    DIM_BLOCK=256
    NPROW=4
    NPCOL=4
fi

./build/hpc-summa.out $N $DIM_BLOCK $NPROW $NPCOL
