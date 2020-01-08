#pragma once

template <typename T>
class Search
{
public:
	static int GetIndexByBinarySearchRecursive(T bArray[], int firstIndex, int lastIndex, T targetData)
	{
		if (firstIndex > lastIndex)
		{
			return -1;
		}

		int midIndex = (firstIndex + lastIndex) / 2;
		if (bArray[midIndex] == targetData)
		{
			return midIndex;
		}
		else if (bArray[midIndex] > targetData)
		{
			return GetIndexBinarySearchRecursive(bArray, firstIndex, midIndex - 1, targetData);
		}
		else
		{
			return GetIndexBinarySearchRecursive(bArray, midIndex + 1, lastIndex, targetData);
		}
	}

	static int GetIndexByInterpolationSearchRecursive(T bArray[], int firstIndex, int lastIndex, T targetData)
	{
		if (bArray[firstIndex] > targetData || bArray[lastIndex] < targetData)
		{
			return -1;
		}

		int midIndex = static_cast<double>(targetData - bArray[firstIndex]) / (bArray[lastIndex] - bArray[firstIndex]) * 
			(lastIndex - firstIndex) + firstIndex;

		if (bArray[midIndex] == targetData)
		{
			return midIndex;
		}
		else if (bArray[midIndex] > targetData)
		{
			return GetIndexByInterpolationSearchRecursive(bArray, firstIndex, midIndex - 1, targetData);
		}
		else
		{
			return GetIndexByInterpolationSearchRecursive(bArray, midIndex + 1, lastIndex, targetData);
		}
	}
};