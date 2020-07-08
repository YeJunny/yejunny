#pragma once

template <typename T>
class QuickSort
{
public:
	static void SortMid(T arr[], int low, int high)
	{
		if (low < high)
		{
			int pi = PartitionMid(arr, low, high);
			if (low < pi - 1)
			{
				SortMid(arr, low, pi - 1);
			}
			else if (high > pi)
			{
				SortMid(arr, pi, high);
			}
		}
	}

	static void SortLast(T arr[], int low, int high)
	{
		if (low < high)
		{
			int pi = PartitionLast(arr, low, high);
			SortLast(arr, low, pi - 1);
			SortLast(arr, pi + 1, high);
		}
	}

private:
	static int PartitionLast(T arr[], int low, int high)
	{
		int i = -1;

		for (int j = 0; j < high; ++j)
		{
			if (arr[j] < arr[high])
			{
				++i;
				Swap(&arr[i], &arr[j]);
			}
		}

		Swap(&arr[i + 1], &arr[high]);

		return i + 1;
	}
	
	static int PartitionMid(T arr[], int low, int high)
	{
		T pivot = arr[(low + high) / 2];

		while (low < high)
		{
			while (arr[low] < pivot)
			{
				++low;
			}

			while (arr[high] > pivot)
			{
				--high;
			}

			if (low < high)
			{
				Swap(&arr[low], &arr[high]);
				++low;
				--high;
			}
		}
		return low;
	}

	static void Swap(int* a, int* b)
	{
		T temp = *a;
		*a = *b;
		*b = temp;
	}
};