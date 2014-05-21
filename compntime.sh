#/bin/bash

gcc matmul.c -O3 -maltivec -o matmul.x && ./matmul.x 1 > /dev/null
echo "NO-OP"
time ./matmul.x 1
echo "DO-OP2"
./matmul.x 2 > /dev/null
time ./matmul.x 2
echo "DO-OP3"
./matmul.x 3 > /dev/null
time ./matmul.x 3 

