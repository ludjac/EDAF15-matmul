gcc -O3 -maltivec matmul.c -o matmul.x 

for b in 32 64 96 128 160 192 224 256 288 320 352 384 416 448 480 512
do
  echo "$b:"
  ./matmul.x 3 $b > /dev/null
  time ./matmul.x 3 $b
done


