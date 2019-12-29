#pragma once

template <typename T>
class MergeSort
{
public:
	static void Sort(T mergeArray[], size_t leftIndex, size_t rightIndex)
	{
		if (leftIndex < rightIndex)
		{
			size_t midIndex = (leftIndex + rightIndex) / 2;

			Sort(mergeArray, leftIndex, midIndex);
			Sort(mergeArray, midIndex + 1, rightIndex);

			int diffIndex = rightIndex - leftIndex;
			T* sortArray = new T[diffIndex + 1];

			size_t nodeLeftIndex = leftIndex;
			size_t nodeRightIndex = midIndex + 1;
			size_t index = 0;

			while (nodeLeftIndex <= midIndex && nodeRightIndex <= rightIndex)
			{
				if (mergeArray[nodeLeftIndex] < mergeArray[nodeRightIndex])
				{
					sortArray[index++] = mergeArray[nodeLeftIndex++];
				}
				else
				{
					sortArray[index++] = mergeArray[nodeRightIndex++];
				}
			}
			
			while (nodeLeftIndex <= midIndex)
			{
				sortArray[index++] = mergeArray[nodeLeftIndex++];
			}

			while (nodeRightIndex <= rightIndex)
			{
				sortArray[index++] = mergeArray[nodeRightIndex++];
			}

			/*for (int i = 0; i <= diffIndex; ++i)
			{
				std::cout << sortArray[i] << " ";
			}
			std::cout << std::endl;*/

			for (int i = 0; i <= diffIndex; ++i)
			{
				mergeArray[leftIndex + i] = sortArray[i];
			}

			delete[] sortArray;
		}
	}
};