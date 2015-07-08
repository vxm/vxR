#ifndef SSEUTILS_H
#define SSEUTILS_H

#include <xmmintrin.h>
#include <pmmintrin.h>

#ifndef __SSE2__
	error "SSE2 instruction set not enabled"
#endif

#define N 10000

double a[N] __attribute__((aligned(16)));
double b[N] __attribute__((aligned(16)));
double c[N] __attribute__((aligned(16)));
double r[N] __attribute__((aligned(16)));

#define NTIMES 100000
int testSSE(void)
{
	int i, times;
	for( times = 0; times < NTIMES; times++ )
	{
		for( i = 0; i <N; i+= 2)
		{
			__m128d mm_a = _mm_load_pd( &a[i] );  
			_mm_prefetch( &a[i+4], _MM_HINT_T0 );
			__m128d mm_b = _mm_load_pd( &b[i] );  
			_mm_prefetch( &b[i+4] , _MM_HINT_T0 );
			__m128d mm_c = _mm_load_pd( &c[i] );
			_mm_prefetch( &c[i+4] , _MM_HINT_T0 );
			__m128d mm_r;
			mm_r = _mm_add_pd( mm_a, mm_b );
			mm_a = _mm_mul_pd( mm_r , mm_c );
			_mm_store_pd( &r[i], mm_a );
		}
	}
	
	return 0;
}


class sseUtils
{
public:
	sseUtils();
};

#endif // SSEUTILS_H
