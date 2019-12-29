#pragma once
using FunPtr = void (*)(int);

template <typename T>
class SimpleBTree
{
public:
	SimpleBTree()
		: mData(0)
		, mLeftNode(nullptr)
		, mRightNode(nullptr)
	{
	}

	SimpleBTree(T& data)
		: mData(data)
		, mLeftNode(nullptr)
		, mRightNode(nullptr)
	{
	}

	~SimpleBTree()
	{
	}

	void MakeLeftSubTree(SimpleBTree* node)
	{
		mLeftNode->DeleteBTree();

		mLeftNode = node;
	}

	void MakeRightSubTree(SimpleBTree* node)
	{
		mRightNode->DeleteBTree();

		mRightNode = node;
	}

	void SearchPreOrderBTree(FunPtr funPtr) const
	{
		if (this == nullptr)
		{
			return;
		}

		funPtr(mData);
		mLeftNode->SearchPreOrderBTree(funPtr);
		mRightNode->SearchPreOrderBTree(funPtr);
	}

	void SearchInOrderBTree(FunPtr funPtr) const
	{
		if (this == nullptr)
		{
			return;
		}

		mLeftNode->SearchInOrderBTree(funPtr);
		funPtr(mData);
		mRightNode->SearchInOrderBTree(funPtr);
	}

	void SearchPostOrderBTree(FunPtr funPtr) const
	{
		if (this == nullptr)
		{
			return;
		}

		mLeftNode->SearchPostOrderBTree(funPtr);
		mRightNode->SearchPostOrderBTree(funPtr);
		funPtr(mData);
	}

	void DeleteBTree() const
	{
		if (this == nullptr)
		{
			return;
		}

		mLeftNode->DeleteBTree();
		mRightNode->DeleteBTree();
		delete this;
	}

	SimpleBTree* GetLeftTree() const
	{
		return mLeftNode;
	}

	SimpleBTree* GetRightTree() const
	{
		return mRightNode;
	}

	T GetData() const
	{
		return mData;
	}


private:
	SimpleBTree* mLeftNode;
	SimpleBTree* mRightNode;
	T mData;
};

