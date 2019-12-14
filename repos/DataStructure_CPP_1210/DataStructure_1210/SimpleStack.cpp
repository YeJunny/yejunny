#include "SimpleStack.h"

SimpleStack::SimpleStack()
{
	mLinkedList = new LinkedList();
}

SimpleStack::~SimpleStack()
{
	delete mLinkedList;
}

void SimpleStack::Push(DataType data) const
{
	mLinkedList->Insert(data);
}

bool SimpleStack::Pop(DataType* outData) const
{
	mLinkedList->SetCurrentPositionTail();

	DataType data;
	bool bIsEnd = mLinkedList->Remove(&data);

	if (outData)
	{
		*outData = data;
	}

	return bIsEnd;
}

DataType SimpleStack::View() const
{
	mLinkedList->SetCurrentPositionTail();
	return mLinkedList->GetData();
}