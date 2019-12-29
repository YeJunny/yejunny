#pragma once

template <typename T>
class InsertionSort
{
public:
	static void Sort(T insertArray[], size_t arraySize)
	{
		size_t insIndex;

		for (int i = 1; i < arraySize; ++i)
		{
			T insData = insertArray[i];
			insIndex = 0;

			for (int j = i - 1; j >= 0; --j)
			{
				if (insertArray[j] > insData)
				{
					insertArray[j + 1] = insertArray[j];
				}
				else
				{
					insIndex = j + 1;
					break;
				}
			}

			insertArray[insIndex] = insData;
		}
	}
};