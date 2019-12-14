#include "LinkedList.h"

struct Node
{
	Node* Next = nullptr;
	DataType Data = 0;
};

LinkedList::LinkedList()
	: mCurrent(nullptr)
	, mHead(nullptr)
	, mTail(nullptr)
{
}

LinkedList::~LinkedList()
{
	First();
	Remove();
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
		mTail = mHead;
	}
	else
	{
		Node* node = new Node();
		node->Data = data;

		node->Next = mCurrent->Next;
		mCurrent->Next = node;
		mCurrent = node;

		if (!mCurrent->Next)
		{
			mTail = node;
		}
	}
}

bool LinkedList::First(DataType* outData)
{	
	mCurrent = mHead;

	if (!mCurrent)
	{
		return false;
	}

	if (outData)
	{
		*outData = mCurrent->Data;
	}

	return true;
}

bool LinkedList::Next(DataType* outData)
{
	if (!mCurrent || !mCurrent->Next)
	{
		return false;
	}

	mCurrent = mCurrent->Next;
	if (outData)
	{
		*outData = mCurrent->Data;
	}
	
	return true;
}

bool LinkedList::Remove(DataType* outData)
{
	if (!mCurrent)
	{
		return false;
	}

	if (outData)
	{
		*outData = mCurrent->Data;
	}

	if (mCurrent == mHead)
	{
		mHead = mCurrent->Next;
		delete mCurrent;
		mCurrent = mHead;
		mTail = mHead;

		return true;
	}

	Node* preNode = mHead;
	while (true)
	{
		if (preNode->Next == mCurrent)
		{
			break;
		}
		preNode = preNode->Next;
	}

	if (!mCurrent->Next)
	{
		delete mCurrent;
		preNode->Next = nullptr;
		mCurrent = preNode;
		mTail = preNode;
	}
	else
	{
		preNode->Next = mCurrent->Next;
		delete mCurrent;
		mCurrent = preNode;
	}

	return true;
}

void LinkedList::SetCurrentPositionHead()
{
	mCurrent = mHead;
}

void LinkedList::SetCurrentPositionTail()
{
	mCurrent = mTail;
}

DataType LinkedList::GetData() const
{
	if (mCurrent)
	{
		return mCurrent->Data;
	}
	else
	{
		return -1;
	}
}