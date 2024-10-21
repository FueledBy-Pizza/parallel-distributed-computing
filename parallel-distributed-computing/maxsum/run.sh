#!/bin/bash

if [ ! -x ./build/maxsum.out ]; then
    echo "Executable not found. Exiting."
    exit 1
fi

if [ -f config.sh ]; then
    source config.sh
else
    MATRIX_ORDER=200   
fi

./build/maxsum.out "$MATRIX_ORDER"
