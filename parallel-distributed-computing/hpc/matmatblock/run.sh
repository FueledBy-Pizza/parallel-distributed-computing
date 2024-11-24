#!/bin/bash

if [ ! -x ./build/hpc-matmatblock.out ]; then
    echo "Executable not found. Exiting."
    exit 1
fi

if [ -f config.sh ]; then
    source config.sh
else
    N=128
fi

./build/hpc-matmatblock.out $N
