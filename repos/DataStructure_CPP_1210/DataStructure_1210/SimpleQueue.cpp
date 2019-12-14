#include "SimpleQueue.h"

SimpleQueue::SimpleQueue()
{
	mLinkedList = new LinkedList();
}

SimpleQueue::~SimpleQueue()
{
	delete mLinkedList;
}

void SimpleQueue::Push(DataType data) const
{
	mLinkedList->Insert(data);
}

bool SimpleQueue::Pop(DataType* outData) const
{
	mLinkedList->SetCurrentPositionHead();

	DataType data;
	bool bIsEnd = mLinkedList->Remove(&data);

	if (outData)
	{
		*outData = data;
	}

	return bIsEnd;
}