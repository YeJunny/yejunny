#pragma once
#include "LinkedList.h"

template <typename T>
class SimpleQueue
{
public:
	SimpleQueue()
	{
		mLinkedList = new LinkedList<T>();
	}

	~SimpleQueue()
	{
		delete mLinkedList;
	}

	void Push(T data) const
	{
		mLinkedList->Insert(data);
	}

	bool Pop(T* outData = nullptr) const
	{
		mLinkedList->SetCurrentPositionHead();

		T data;
		bool bIsEnd = mLinkedList->Remove(&data);

		if (outData)
		{
			*outData = data;
		}

		return bIsEnd;
	}

private:
	LinkedList<T>* mLinkedList;
};

