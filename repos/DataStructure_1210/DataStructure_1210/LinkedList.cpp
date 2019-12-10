#include "LinkedList.h"

struct Node
{
	Node* Next = nullptr;
	DataType Data = 0;
};

LinkedList::LinkedList()
	: mCurrent(nullptr)
	, mHead(nullptr)
{
}

LinkedList::~LinkedList()
{
	while (Next())
	{
		Remove();
	}
}

void LinkedList::Insert(DataType data)
{
	if (!mHead)
	{
		mHead = new Node();
		mHead->Data = data;
		mCurrent = mHead;
	}
	else
	{
		Node* node = new Node();
		node->Data = data;
		node->Next = mCurrent->Next;
		mCurrent->Next = node;
		mCurrent = node;
	}
}

bool LinkedList::Next(DataType* outData)
{
	static bool bCycle = false;
	static Node* startNode = mCurrent;
	
	if (!mCurrent || bCycle)
	{
		return false;
	}

	if (!mCurrent->Next)
	{
		mCurrent = mHead;
		if (outData)
		{
			*outData = mCurrent->Data;
		}
	}
	else
	{
		if (outData)
		{
			*outData = mCurrent->Next->Data;
		}
		mCurrent = mCurrent->Next;
	}

	if (startNode == mCurrent)
	{
		bCycle = true;
	}
	
	return true;
}

void LinkedList::Remove()
{
	if (!mCurrent)
	{
		return;
	}
	else if (!mCurrent->Next)
	{
		mHead = mCurrent->Next;
		delete mCurrent;
		mCurrent = mHead;
	}
	else
	{

	}
}