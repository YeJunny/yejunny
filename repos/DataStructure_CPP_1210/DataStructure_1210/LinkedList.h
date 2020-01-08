#pragma once

template <typename T>
struct Node
{
	Node* Next;
	T Data;
};

template <typename T>
class LinkedList
{
public:
	LinkedList()
		: mCurrent(nullptr)
		, mHead(nullptr)
		, mTail(nullptr)
	{
	}

	~LinkedList()
	{
		First();
		Remove();
		while (Next())
		{
			Remove();
		}
	}

	void Insert(T data)
	{
		if (!mHead)
		{
			mHead = new Node<T>();
			mHead->Data = data;
			mCurrent = mHead;
			mTail = mHead;
		}
		else
		{
			Node<T>* node = new Node<T>();
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

	bool First(T* outData = nullptr)
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

	bool Next(T* outData = nullptr)
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

	bool Remove(T* outData = nullptr)
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

		Node<T>* preNode = mHead;
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

	void SetCurrentPositionHead()
	{
		mCurrent = mHead;
	}

	void SetCurrentPositionTail()
	{
		mCurrent = mTail;
	}

	T GetData() const
	{
		if (mCurrent)
		{
			return mCurrent->Data;
		}
		else
		{
			return 0;
		}
	}

private:
	Node<T>* mHead;
	Node<T>* mCurrent;
	Node<T>* mTail;
};

