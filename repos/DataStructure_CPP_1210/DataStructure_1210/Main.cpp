#include <iostream>
#include "Math.h"
#include "RadixSort.h"
using namespace std;

int main()
{
	int numbers[] = { 15,24,13,8,11,1456,2,0,1,9 };
	//int numbers[] = { 3,3,3 };
	//int numbers[] = { 3,2,4,1,7,8,2 };

	RadixSort<int>::Sort(numbers, sizeof(numbers) / sizeof(int), 4);

	for (int number : numbers)
	{
		cout << number << " ";
	}
}
