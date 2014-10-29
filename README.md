Matrix Multiplication Optimization
==================================

Ludwig Jacobsson

Optimizations
-------------

  1. Change order of for-loops to reduce cache misses and enable 
     vectorization.
  2. Tiling / Blocking for loop to make us of L1 cache size and 
     cache-line
  3. Attempted unrolling inner loop but it had a negtive effect

General info
------------

    sizeof(float)*N | 4*512 = 2048 

    1 row is       2048 bytes
    512 rows is    1048576 bytes ~ 1048 Kbytes
    L1 is          32000   bytes - 32   kbytes
        line size  128     bytes
    L2 is          1024000 bytes - 1024 Kbytes
        line size  128     bytes

Optimal blocksize for L1 cache
------------------------------
blocksize relation

    2*(blocksize)^2*wordsize = cachesize

**Blocksize L2 cache:**

    blocksize_L2 = (1024000/(2*4))^(1/2) ~= 358 elements

**Fit two matrix blocks into L1 cache:**
    
    blocksize_L1 = (L1size/(2*wordsize))^(1/2)
    blocksize_L1 = (32000/(2*4))^(1/2) ~= 64 elements

**Cache-line L1/L2 128 bytes**

    128 bytes -> 128/4 elements = 32 elements

We want blocksize to be a multiple of the cache-line ie: 
    
    32*n for n=1, 2, 3, ...

**From experiment**

    32      64      96      128     160     192     224     256   (blocksize)
    0.162   0.127   0.106   0.092   0.093   0.086   0.088   0.080 (sec)

    288     320     352     384     416     448     480     512   (blocksize)
    0.080   0.081   0.080   0.100   0.010   0.216   0.177   0.158 (sec)

Optimal blocksize seems to be ~ 256-352 elements
(This, is not really in line with my calculations, dont know why... :(
Perhaps this is blocked for the L2 cache
    256*256*4byte*2 ~= 0.5Kb ~ L2/2

NO-TODO: Try to recursively optimize for L1 in inner loop - negative impact

Cache miss/hits for matmul3, Blocksize 320
------------------------------------------
   
    ==24740== I   refs:      312,092,182
    ==24740== I1  misses:            873
    ==24740== LLi misses:            871
    ==24740== I1  miss rate:        0.00%
    ==24740== LLi miss rate:        0.00%
    ==24740== 
    ==24740== D   refs:      107,529,146  (71,862,221 rd   + 35,666,925 wr)
    ==24740== D1  misses:      9,478,651  ( 9,477,048 rd   +      1,603 wr)
    ==24740== LLd misses:      9,475,388  ( 9,474,797 rd   +        591 wr)
    ==24740== D1  miss rate:         8.8% (      13.1%     +        0.0%  )
    ==24740== LLd miss rate:         8.8% (      13.1%     +        0.0%  )
    ==24740== 
    ==24740== LL refs:         9,479,524  ( 9,477,921 rd   +      1,603 wr)
    ==24740== LL misses:       9,476,259  ( 9,475,668 rd   +        591 wr)
    ==24740== LL miss rate:          2.2% (       2.4%     +        0.0%  )

**TODO: Strassen algortimh?**

Testbranch change org
Testbranch change 2
Testbranch2 change 1
