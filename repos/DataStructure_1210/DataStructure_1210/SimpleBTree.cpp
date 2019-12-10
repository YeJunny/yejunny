#include "SimpleBTree.h"
#include <iostream>

SimpleBTree::SimpleBTree()
	: mData(0)
	, mLeftNode(nullptr)
	, mRightNode(nullptr)
{
}

SimpleBTree::SimpleBTree(DataType data)
	: mData(data)
	, mLeftNode(nullptr)
	, mRightNode(nullptr)
{
}

void SimpleBTree::MakeLeftSubTree(SimpleBTree* node)
{
	mLeftNode->DeleteBTree();

	mLeftNode = node;
}

void SimpleBTree::MakeRightSubTree(SimpleBTree* node)
{
	mRightNode->DeleteBTree();

	mRightNode = node;
}

void SimpleBTree::SearchPreOrderBTree(FunPtr funPtr) const
{
	if (this == nullptr)
	{
		return;
	}

	funPtr(mData);
	mLeftNode->SearchPreOrderBTree(funPtr);
	mRightNode->SearchPreOrderBTree(funPtr);
}

void SimpleBTree::SearchInOrderBTree(FunPtr funPtr) const
{
	if (this == nullptr)
	{
		return;
	}

	mLeftNode->SearchInOrderBTree(funPtr);
	funPtr(mData);
	mRightNode->SearchInOrderBTree(funPtr);
}

void SimpleBTree::SearchPostOrderBTree(FunPtr funPtr) const
{
	if (this == nullptr)
	{
		return;
	}

	mLeftNode->SearchPostOrderBTree(funPtr);
	mRightNode->SearchPostOrderBTree(funPtr);
	funPtr(mData);
}

SimpleBTree::~SimpleBTree()
{
}

void SimpleBTree::DeleteBTree() const
{
	if (this == nullptr)
	{
		return;
	}

	mLeftNode->DeleteBTree();
	mRightNode->DeleteBTree();
	delete this;
}