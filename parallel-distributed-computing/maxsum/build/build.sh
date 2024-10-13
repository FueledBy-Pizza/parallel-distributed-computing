#!/bin/bash

cc -Xclang -fopenmp \
   -I/opt/homebrew/opt/libomp/include \
   -L/opt/homebrew/opt/libomp/lib \
   ../../common/c_timer.c \
   ../src/matrix_ops/matrix_ops.c \
   ../src/maxsum/maxsum.c \
   ../main.c \
   -lomp \
   -o maxsum.out

if [ $? -ne 0 ]; then
    echo "Build failed. Exiting."
    exit 1
fi
