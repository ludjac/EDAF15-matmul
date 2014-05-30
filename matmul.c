/* Anything may be changed, except matrix sizes and float type. 
 *
 * The output when defining CHECK should be the same with and without tuning.
 *
 * Optimizer: Ludwig Jacobsson, se matmul.org.c for original code 
 *
 * orignal time ~ 0.812
 * OPTIMIZATIONS:
 *  1. Change order of for-loops to reduce cache misses and enable 
 *     vectorization. ~ 0.154
 *  2. Tiling / Blocking for loop to make us of L1 cache size and 
 *     cache-line. ~ 0.080
 *
 *    TODO: Unroll inner loops  -- negative effect
 *    TODO: Cache optimize for L1 in inner loop -- Negative effect
 *    TODO: Analyse assembler code for inefficient code
 *    TODO: Strassen algortimh?
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define CHECK (1)
#define N (512)
#define blocksize (320)

#ifdef CHECK
#define TYPE	int // 4 bytes
#else
#define TYPE	float // 4 bytes
#endif

TYPE a[N][N];
TYPE b[N][N];
TYPE c[N][N];

size_t min(size_t a, size_t b) 
{
        return (a < b ? a : b);
}
/* Blocked for L1 (?) and cache miss optimized traversal (row-major)  */
void matmul3()
{
	size_t	i, j, k, ii, jj, kk, iii, jjj, kkk ;
        
        memset(a, '\0', N);

	for (i = 0; i < N; i += blocksize) {
	  for (j = 0; j < N; j += blocksize) {
	    for (k = 0; k < N; k += blocksize) {
	      /* Multiply block elements */
              for (ii = i; ii < min(N, i + blocksize); ii += 1){
                for (jj = j; jj < min(N, j + blocksize); jj += 1){
                  for (kk = k; kk < min(N, k + blocksize); kk += 1){ 
                    a[jj][kk] += b[jj][ii] * c[ii][kk];
                  }
                }
              }
            }
	  }
	}
}

/* Cache miss optimized */
void matmul2()
{
        memset(a, '\0', N);

	size_t	i, j, k;
	
        for (i = 0; i < N; i += 1) {
		for (j = 0; j < N; j += 1) {
                        // a[i][j] = 0;  -- Replaced with memset
                        //tmp = b[j][i];  -- Replaced with -O3 opt-flag 
			for (k = 0; k < N; k += 1)
				a[j][k] += b[j][i] * c[i][k];
		}
	}

}
/* Original */
void matmul1()
{
	size_t	i, j, k;

	for (i = 0; i < N; i += 1) {
		for (j = 0; j < N; j += 1) {
			a[i][j] = 0;
			for (k = 0; k < N; k += 1)
				a[i][j] += b[i][k] * c[k][j];
		}
	}
}

void init()
{
#ifdef CHECK
	size_t	i, j;

	for (i = 0; i < N; i += 1) {
		for (j = 0; j < N; j += 1) {
			b[i][j] = 12 + i * j * 13;
			c[i][j] = -13 + i + j * 21;
		}
	}
#endif
}

void output()
{
#ifdef CHECK
	size_t	i, j;

	for (i = 0; i < N; i += 1)
		for (j = 0; j < N; j += 1)
			printf("a[%3zu][%3zu] = %d\n", i, j, a[i][j]);
#endif
}

int main(int argc, char** argv)
{
        int mul = 0;
        int opt = 32;
        

        if (argc > 1) {
                sscanf(argv[1], "%d", &mul);
        }

        if (argc > 2) {
                sscanf(argv[2], "%d", &opt);
        }

       	init();
	
        if (mul == 1){
                //printf("NO-OP");
                matmul1();
        } else if (mul == 2) {
                //printf("DO-OP");
                matmul2();
        } else if (mul == 3) {
                //printf("DO-OP");
                matmul3();
        } else {
                printf("NADA");
                exit(1);
        }

	output();

	return 0;
}
