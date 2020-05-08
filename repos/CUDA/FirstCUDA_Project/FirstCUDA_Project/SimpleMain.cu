#include <iostream>
//#include "cuda.h"

using namespace std;

__global__ void AddNumber(int* dest, int* source)
{
	*dest += *source;
}

int main() 
{
	int number1 = 5;
	int number2 = 9;

	int* destNumber1;
	int* destNumber2;

	if (cudaMalloc(&destNumber1, sizeof(int)) == cudaSuccess)
	{
		printf("Success\n");
	}
	if (cudaMalloc(&destNumber2, sizeof(int)) == cudaSuccess)
	{
		printf("Success\n");
	}

	cudaMemcpy(destNumber1, &number1, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(destNumber2, &number2, sizeof(int), cudaMemcpyHostToDevice);

	AddNumber << <1, 1 >> > (destNumber1, destNumber2);

	cudaMemcpy(&number1, destNumber1, sizeof(int), cudaMemcpyDeviceToHost);

	cout << "Result AddNumber() : " << number1 << endl;

	cudaFree(destNumber1);
	cudaFree(destNumber2);

	return 0;
}