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

	SimpleBTree(const T& data)
		: mData(data)
		, mLeftNode(nullptr)
		, mRightNode(nullptr)
	{
	}

	~SimpleBTree()
	{
	}

	void MakeLeftSubTree(SimpleBTree<T>* node)
	{
		mLeftNode->DeleteBTreeRecursive();

		mLeftNode = node;
	}

	void MakeRightSubTree(SimpleBTree<T>* node)
	{
		mRightNode->DeleteBTreeRecursive();

		mRightNode = node;
	}

	void SearchPreOrderBTreeRecursive(const FunPtr funPtr) const
	{
		if (this == nullptr)
		{
			return;
		}

		funPtr(mData);
		mLeftNode->SearchPreOrderBTreeRecursive(funPtr);
		mRightNode->SearchPreOrderBTreeRecursive(funPtr);
	}

	void SearchInOrderBTreeRecursive(const FunPtr funPtr) const
	{
		if (this == nullptr)
		{
			return;
		}

		mLeftNode->SearchInOrderBTreeRecursive(funPtr);
		funPtr(mData);
		mRightNode->SearchInOrderBTreeRecursive(funPtr);
	}

	void SearchPostOrderBTreeRecursive(const FunPtr funPtr) const
	{
		if (this == nullptr)
		{
			return;
		}

		mLeftNode->SearchPostOrderBTreeRecursive(funPtr);
		mRightNode->SearchPostOrderBTreeRecursive(funPtr);
		funPtr(mData);
	}

	void DeleteBTreeRecursive() const
	{
		if (this == nullptr)
		{
			return;
		}

		mLeftNode->DeleteBTreeRecursive();
		mRightNode->DeleteBTreeRecursive();
		delete this;
	}

	void SetLeftTree(SimpleBTree<T>* leftNode)
	{
		mLeftNode = leftNode;
	}

	void SetRightTree(SimpleBTree<T>* rightNode)
	{
		mRightNode = rightNode;
	}

	SimpleBTree<T>* GetLeftTree() const
	{
		return mLeftNode;
	}

	SimpleBTree<T>* GetRightTree() const
	{
		return mRightNode;
	}

	void SetData(const T data)
	{
		mData = data;
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

