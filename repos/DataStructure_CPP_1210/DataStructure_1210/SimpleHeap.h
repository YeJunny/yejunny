#pragma once

#define MAX_LEN (100)

template <typename T>
class SimpleHeap
{
public:
	typedef int GetPriority(T data1, T data2);

	SimpleHeap()
		: mIndex(1)
	{
	}

	void Init(GetPriority getPriorityFunc, bool bIsMaxHeap)
	{
		mbIsMaxHeap = bIsMaxHeap;
		mPriorityFunc = getPriorityFunc;
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
		
		if (mbIsMaxHeap && mPriorityFunc(mHeapArray[index], mHeapArray[parentIndex]) > 0 ||
			!mbIsMaxHeap && mPriorityFunc(mHeapArray[index], mHeapArray[parentIndex]) < 0)
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

		if (mbIsMaxHeap && mPriorityFunc(leftChildValue, currentNodeValue) > 0 && mPriorityFunc(leftChildValue, rightChildValue) > 0 ||
			!mbIsMaxHeap && mPriorityFunc(leftChildValue, currentNodeValue) < 0 && mPriorityFunc(leftChildValue, rightChildValue) < 0)
		{
			Swap(index, leftChildIndex);
			RemoveNodeRecursive(leftChildIndex);
		}
		else if (mbIsMaxHeap && mPriorityFunc(rightChildValue, currentNodeValue) > 0 ||
			!mbIsMaxHeap && mPriorityFunc(rightChildValue, currentNodeValue) < 0)
		{
			Swap(index, rightChildIndex);
			RemoveNodeRecursive(rightChildIndex);
		}
	}

	void Swap(int& index1, int& index2)
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

	T mHeapArray[MAX_LEN];
	GetPriority* mPriorityFunc;
	bool mbIsMaxHeap;
	int mIndex;
};