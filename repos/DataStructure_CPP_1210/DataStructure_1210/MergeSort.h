#pragma once

template <typename T>
class MergeSort
{
public:
	static void Sort(T arr[], int l, int r)
	{
		if (l < r)
		{
			int mid = (l + r) / 2;
			Sort(arr, l, mid);
			Sort(arr, mid + 1, r);
			Merge(arr, l, mid, r);
		}
	}

private:
	static void Merge(T arr[], int l, int m, int r)
	{
		const int num = r - l + 1;

		int i = l, j = m + 1, k = 0;
		T* temp = new T[num];

		while (i <= m && j <= r)
		{
			if (arr[i] < arr[j])
			{
				temp[k++] = arr[i++];
			}
			else
			{
				temp[k++] = arr[j++];
			}
		}

		while (i <= m)
		{
			temp[k++] = arr[i++];
		}

		while (j <= r)
		{
			temp[k++] = arr[j++];
		}

		for (int n = 0; n < num; ++n)
		{
			arr[l + n] = temp[n];
		}

		delete[] temp;
	}
};