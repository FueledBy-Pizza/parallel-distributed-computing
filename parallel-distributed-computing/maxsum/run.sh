#!/bin/bash

source config.sh

if [ ! -x ./build/maxsum.out ]; then
    echo "Executable not found. Exiting."
    exit 1
fi

./build/maxsum.out "$MATRIX_ORDER"
