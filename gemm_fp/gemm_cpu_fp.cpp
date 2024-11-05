#ifdef __cplusplus
#define PUT_IN_REGISTER
#else
#define PUT_IN_REGISTER register
#endif
#include "gemm_cpu_fp.h"

void gemm_cpu_fp(int TA, int TB, int M,  int N, int K, float ALPHA,
        float *A, int lda,
        float *B, int ldb,
        float BETA,
        float *C, int ldc)
{
    int i,j,k;
    for(i = 0; i < M; ++i){
        for(k = 0; k < K; ++k){
            PUT_IN_REGISTER float A_PART = ALPHA * A[i * lda + k];
            for(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}
void gemm_cpu_fixed_p(int TA, int TB, int M, int N, int K, int ALPHA,
        int64_t *A, int lda,
        int64_t *B, int ldb,
        int BETA,
        int64_t *C, int ldc, int scale)
        {
        int i,j,k;
        for(i = 0; i < M; ++i){
            for(k = 0; k < K; ++k){
                PUT_IN_REGISTER int64_t A_PART = (ALPHA * A[i * lda + k]);
                for(j = 0; j < N; ++j){
                    C[i*ldc+j] += (A_PART*B[k*ldb+j]) >> scale;
            }
        }
    }
}

int roundup(float fp_number)
{
	int	fx_number =	(int)fp_number;

	if(fp_number-fx_number>=0.5)
	{
		fx_number++;
	}
	return fx_number;
}
void mm_float_to_fixed(float* A, int64_t* B, int dimA, int dimB, int lda, int scale)
{
    for(int i = 0;i < dimA; i++)
	{
		for(int j = 0;j < dimB; j++)
		{
			B[i*lda+j] = roundup(A[i*lda+j]*(1<<scale));
		}
	}
}
void mm_fixed_to_float(int64_t* A, float* B, int dimA, int dimB, int lda, int scale)
{
    for(int i = 0;i < dimA; i++)
	{
		for(int j = 0;j < dimB; j++)
		{
			B[i*lda+j] = (float)(A[i*lda+j])/(1<<scale);
		}
	}
}

int float_to_fixed(float float_value, int scale) {
    return roundup(float_value * (1 << scale));
}

float fixed_to_float(int64_t fixed_value, int scale) {
    return (float)fixed_value / (1 << scale);
}