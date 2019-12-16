#pragma once
#include "LinkedList.h"

template <typename T>
class SimpleStack
{
public:
	SimpleStack()
	{
		mLinkedList = new LinkedList<T>();
	}

	~SimpleStack()
	{
		delete mLinkedList;
	}

	void Push(T data) const
	{
		mLinkedList->Insert(data);
	}

	bool Pop(T* outData = nullptr) const
	{
		mLinkedList->SetCurrentPositionTail();

		T data;
		bool bIsEnd = mLinkedList->Remove(&data);

		if (outData)
		{
			*outData = data;
		}

		return bIsEnd;
	}

	T View() const
	{
		mLinkedList->SetCurrentPositionTail();
		return mLinkedList->GetData();
	}

private:
	LinkedList<T>* mLinkedList;
};

