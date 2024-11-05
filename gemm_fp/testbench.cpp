#include <stdio.h>
#include <math.h>
#include <iostream>
#include "gemm_cpu_fp.h"
#include "testData.h"
using namespace std;

void testFunctionRef();
void compareMatrix(float* matrixA, float* matrixB, int dimA, int dimB, int lda, int ldb);
void printMatrix(int *A, int dimA, int dimB, int lda);
void printMatrix(float *A, int dimA, int dimB, int lda);
float calculatematrixSNR(float* matrixA, float* matrixB, int dimA, int dimB, int lda, int ldb);

int main(int argc, char* argv[])
{
	int TA = 0;
	int TB = 0; 
	float ALPHA = 1;
	int lda = 1;
	int ldb = tst_dim_N;
	int ldc = 1;
	float BETA = 1;

	gemm_cpu_fp(TA, TB, tst_dim_M, 
				tst_dim_N, tst_dim_K, ALPHA, 
				tst_matrix_A, lda,
				tst_matrix_B, ldb,
				BETA,
				tst_matrix_Cin, ldc);

	testFunctionRef();
	compareMatrix(tst_matrix_Cin, tst_matrix_Cout, tst_dim_M, tst_dim_N, lda, lda);
	
	float SNR_FP = calculatematrixSNR(tst_matrix_Cin, tst_matrix_Cout, tst_dim_M, tst_dim_N, lda, lda);
	cout<<"SNR of the floating point number is: "<<SNR_FP << endl;

	//printMatrix(tst_matrix_Cin,dimA,dimB,1);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Now convert the data to fixed point

	int scale = 27;
	cout<<"The scale factor used is: "<<scale<<endl;

	int64_t tst_matrix_A_int[tst_dim_M*tst_dim_K];
	int64_t tst_matrix_B_int[tst_dim_K*tst_dim_N];
	int64_t tst_matrix_C_int[tst_dim_M*tst_dim_N];

	float tst_matrix_C_out[tst_dim_M*tst_dim_N];

	mm_float_to_fixed(tst_matrix_A, tst_matrix_A_int, tst_dim_M, tst_dim_K, lda, scale);
	mm_float_to_fixed(tst_matrix_B, tst_matrix_B_int, tst_dim_K, tst_dim_N, ldb, scale);

	int dim = tst_dim_M*tst_dim_N;
	for (int i = 0; i < dim; i++) 
	{
		tst_matrix_C_int[i] = 0;
	}
	
	gemm_cpu_fixed_p(TA, TB, tst_dim_M, 
					tst_dim_N, tst_dim_K, ALPHA, 
					tst_matrix_A_int, lda,
					tst_matrix_B_int, tst_dim_N,
					BETA,
					tst_matrix_C_int, ldc, scale);

	mm_fixed_to_float(tst_matrix_C_int, tst_matrix_C_out, tst_dim_M, tst_dim_N, lda, scale);
	
	float SNR_FP1 = calculatematrixSNR(tst_matrix_C_out, tst_matrix_Cout, tst_dim_M, tst_dim_N, lda, lda);
	cout<<"SNR of the floating point number is: "<<SNR_FP1 << endl;
}

//Function to print the matrix
void printMatrix(int *A, int dimA, int dimB, int lda)
{
	for(int i = 0; i < dimA; i++)
	{
		for(int j = 0;j < dimB; j++)
		{
			cout<< A[i*lda+j] << " ";
		}
		cout<<endl;
	}
}

//Function to compare results
void compareMatrix(float* matrixA, float* matrixB, int dimA, int dimB, int lda, int ldb)
{
	bool flag = false;
	for(int i = 0; i < dimA; i++)
	{
		for(int j = 0;j < dimB; j++)
		{
			if(abs(matrixA[i*lda+j] - matrixB[i*ldb+j]) > 1e-4)
			{
				flag = true;
				break;
			}
		}
	}
	if(flag)
		cout <<"There is mismatch of the matrix multiplications, please check your mathematics"<<endl;
	else
		cout <<"All values are matching correctly!!"<<endl;
}

float calculatematrixSNR(float* matrixA, float* matrixB, int dimA, int dimB, int lda, int ldb)
{
	float diff = 0.0;
	float sqTerm = 0.0;
	for(int i = 0; i < dimA; i++)
	{
		for(int j = 0; j < dimB; j++)
		{
			diff += pow((matrixA[i*lda+j] - matrixB[i*ldb+j]),2);
			sqTerm += pow((matrixB[i*ldb+j]),2);
		}
	}
	float SNR = 10.0 * log10(sqTerm/diff);
	return SNR;
}
void testFunctionRef()
{
	//Sample example function which was given to understand how the function works
	//We have a sample number 0.213 and we want to convert to fixed point i.e., Integer and multiply 500

	float FloatPointValue = 0.213;
	int scale = 16;

	// convert from floating to fixed point i.e., integer by shifting it right by 2^scale
	int FixedPointValue = roundup(FloatPointValue*(1<<scale));

	//Now multiply with 500
	FixedPointValue = FixedPointValue * 500;

	// Convert back to the floating point value
	float result = (float)(FixedPointValue)/(1<<scale);

	// printing result
	printf("Floating point value:        %.10f\n",FloatPointValue*500);
	printf("Fixed point (Integer) value: %.10f\n",result);
}