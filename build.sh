#!/bin/sh
clang++ ./main.cpp ./src/*.cpp -I ./raylib-5.5/include/ ./raylib-5.5/lib/libraylib.a -g -O0
if [ $? -eq 0 ]; then
    echo "Compile Succeeded at $(date)"
    mv ./a.out ./Making-A-Collection
    ./Making-A-Collection
else
    echo "Command did not run code: $?"
fi
