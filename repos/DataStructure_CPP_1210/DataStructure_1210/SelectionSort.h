#pragma once

template <typename T>
class SelectionSort
{
public:
	static void Sort(T selectArray[], size_t arraySize)
	{
		for (int i = 0; i < arraySize - 1; ++i)
		{
			size_t minIndex = i;

			for (int j = i + 1; j < arraySize; ++j)
			{
				if (selectArray[minIndex] > selectArray[j])
				{
					minIndex = j;
				}
			}

			T tempValue = selectArray[i];
			selectArray[i] = selectArray[minIndex];
			selectArray[minIndex] = tempValue;
		}
	}
};