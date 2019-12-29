#pragma once

#include "SimpleQueue.h"

template <typename T>
class RadixSort
{
public:
	static void Sort(T radixArray[], int arraySize, int maxElemLen)
	{
		SimpleQueue<T>* buckets = new SimpleQueue<T>[10]();

		int divFactor = 1;
		for (int i = 0; i < maxElemLen; ++i)
		{
			for (int j = 0; j < arraySize; ++j)
			{
				int radix = radixArray[j] / divFactor % 10;

				buckets[radix].Push(radixArray[j]);
			}

			int index = 0;
			for (int j = 0; j < 10; ++j)
			{
				T data;
				while (buckets[j].Pop(&data))
				{
					radixArray[index++] = data;
				}
			}

			divFactor *= 10;
		}

		delete[] buckets;
	}
};