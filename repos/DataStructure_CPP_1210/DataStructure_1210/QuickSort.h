#pragma once

template <typename T>
class QuickSort
{
public:
	static void Sort(T quickArray[], int leftIndex, int rightIndex)
	{
		if (leftIndex >= rightIndex)
		{
			return;
		}

		int indexArray[] = { leftIndex, (leftIndex + rightIndex) / 2, rightIndex };

		if (quickArray[indexArray[0]] > quickArray[indexArray[1]])
		{
			Swap(indexArray, 0, 1);
		}
		if (quickArray[indexArray[1]] > quickArray[indexArray[2]])
		{
			Swap(indexArray, 1, 2);
		}
		if (quickArray[indexArray[0]] > quickArray[indexArray[1]])
		{
			Swap(indexArray, 0, 1);
		}

		Swap(quickArray, indexArray[1], leftIndex);

		T pivot = quickArray[leftIndex];
		int low = leftIndex + 1;
		int high = rightIndex;

		while (low <= high)
		{
			while (quickArray[low] <= pivot && low <= rightIndex)
			{
				low++;
			}

			while (quickArray[high] >= pivot && high >= leftIndex + 1)
			{
				high--;
			}

			if (low <= high)
			{
				Swap(quickArray, low, high);
			}
		}

		Swap(quickArray, leftIndex, high);

		Sort(quickArray, leftIndex, high - 1);
		Sort(quickArray, high + 1, rightIndex);
	}

private:
	static void Swap(T quickArray[], size_t index1, size_t index2)
	{
		T tempValue = quickArray[index1];
		quickArray[index1] = quickArray[index2];
		quickArray[index2] = tempValue;
	}
};