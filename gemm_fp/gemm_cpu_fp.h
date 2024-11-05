
/*
1. The values TA, TB are not required however to maintain the same structure of function call in the darknet we preseve those variables
2. M,N,K as mentioned below referenced from the testData.cpp files
// Matrix dimensions. 
// Matrix A = M * K
// Matrix B = K * N
// Matrix C = M * N
3. ALPHA and BETA are the scaling factors. Again we do not require them for the current operations performed
4. lda is the memory offset values of Matrix A
5. similarly B, C have the similar values such as the ldb, ldc
*/
#include <cmath>
#include <cstdint>
int roundup(float fp_number);
void mm_float_to_fixed(float* A, int64_t* B, int dimA, int dimB, int lda, int scale);
void mm_fixed_to_float(int64_t* A, float* B, int dimA, int dimB, int lda, int scale);

void gemm_cpu_fp(int TA, int TB, int M, int N, int K, float ALPHA,
        float *A, int lda,
        float *B, int ldb,
        float BETA,
        float *C, int ldc);

void gemm_cpu_fixed_p(int TA, int TB, int M, int N, int K, int ALPHA,
        int64_t *A, int lda,
        int64_t *B, int ldb,
        int BETA,
        int64_t *C, int ldc, int scale);

float fixed_to_float(int64_t fixed_value, int scale);
int float_to_fixed(float float_value, int scale);