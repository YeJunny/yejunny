#pragma once

#define MAX_LEN (100)

template <typename T>
class SimpleHeap
{
public:
	SimpleHeap()
		: mIndex(1)
	{
	}

	void Init(bool bIsMaxHeap)
	{
		mbIsMaxHeap = bIsMaxHeap;
	}

	void Insert(const T& data)
	{
		mHeapArray[mIndex] = data;
		CompareNodeRecursive(mIndex);
		mIndex++;
	}

	bool Remove(T* outData)
	{
		if (mIndex == 1)
		{
			return false;
		}

		*outData = mHeapArray[1];

		mHeapArray[1] = mHeapArray[mIndex - 1];
		RemoveNodeRecursive(1);
		mIndex--;
		return true;
	}

private:
	void CompareNodeRecursive(int index)
	{
		int parentIndex = GetParentIndex(index);
		if (!parentIndex)
		{
			return;
		}
		
		if (mbIsMaxHeap && mHeapArray[index] > mHeapArray[parentIndex] ||
			!mbIsMaxHeap && mHeapArray[index] < mHeapArray[parentIndex])
		{
			Swap(index, parentIndex);
			CompareNodeRecursive(parentIndex);
		}
	}

	void RemoveNodeRecursive(int index)
	{
		int leftChildIndex = GetLeftChildIndex(index);
		int rightChildIndex = GetRightChildIndex(index);

		if (leftChildIndex >= mIndex || rightChildIndex >= mIndex)
		{
			return;
		}

		T leftChildValue = mHeapArray[leftChildIndex];
		T rightChildValue = mHeapArray[rightChildIndex];
		T currentNodeValue = mHeapArray[index];

		if (mbIsMaxHeap && leftChildValue > currentNodeValue && leftChildValue > rightChildValue ||
			!mbIsMaxHeap && leftChildValue < currentNodeValue && leftChildValue < rightChildValue)
		{
			Swap(index, leftChildIndex);
			RemoveNodeRecursive(leftChildIndex);
		}
		else if (mbIsMaxHeap && rightChildValue > currentNodeValue ||
			!mbIsMaxHeap && rightChildValue < currentNodeValue)
		{
			Swap(index, rightChildIndex);
			RemoveNodeRecursive(rightChildIndex);
		}
	}

	void Swap(int index1, int index2)
	{
		T temp = mHeapArray[index1];
		mHeapArray[index1] = mHeapArray[index2];
		mHeapArray[index2] = temp;
	}

	int GetParentIndex(int index)
	{
		return index / 2;
	}

	int GetLeftChildIndex(int index)
	{
		return index * 2;
	}

	int GetRightChildIndex(int index)
	{
		return index * 2 + 1;
	}

	bool mbIsMaxHeap;
	int mIndex;
	T mHeapArray[MAX_LEN];
};