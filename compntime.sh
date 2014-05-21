#/bin/bash

gcc matmul.c -O3 -maltivec -o matmul.x && ./matmul.x 1 > /dev/null
echo "NO-OP"
time ./matmul.x 1
./matmul.x 2 > /dev/null
echo "DO-OP"
time ./matmul.x 2
