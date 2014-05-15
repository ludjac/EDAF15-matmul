/* Anything may be changed, except matrix sizes and float type. 
 *
 * The output when defining CHECK should be the same with and without tuning.
 *
 * Optimizer: Ludwig Jacobsson, se matmul.org.c for original code 
 *
 * OPTIMIZATIONS:
 * 1. Change order of for-loops to utilize L1 & L2 cache
 */

#include <stdio.h>
#include <string.h>

//#define CHECK (1)
// #define matmul(a)     (mul == a ? matmul##a() : 0)
#define N (512)

#ifdef CHECK
#define TYPE	int
#else
#define TYPE	float
#endif

TYPE a[N][N];
TYPE b[N][N];
TYPE c[N][N];

void matmul2()
{
	size_t	i, j, k;
        TYPE    tmp;
	for (i = 0; i < N; i += 1) {
		for (j = 0; j < N; j += 1) {
			/* This might be trouble, altough globals are initiated to zero (!?) */
                        // a[i][j] = 0; 
                        tmp = b[j][i];
			for (k = 0; k < N; k += 1)
				a[j][k] += tmp * c[i][k];
		}
	}

}
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
        int mul = 1;

        if (argc > 1) {
                sscanf(argv[1], "%d", &mul);
        }
       	init();
	
        if (mul == 1){
                //printf("NO-OP");
                matmul1();
        } else {
                //printf("DO-OP");
                matmul2();
        }
	output();

	return 0;
}
