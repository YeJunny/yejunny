#pragma once

#include "SimpleBTree.h"
#include <assert.h>

void Print(int number)
{
	std::cout << number << " ";
}

template <typename T>
class AVLTree
{
public:
	AVLTree()
		: mRoot(nullptr)
	{
	}

	~AVLTree()
	{
		mRoot->DeleteBTreeRecursive();
	}

	void Insert(const T data)
	{
		if (!mRoot)
		{
			mRoot = new SimpleBTree<T>(data);
		}
		else
		{
			mData = data;
			SearchNodeAndInsertRecursive(mRoot);
		}
	}

	SimpleBTree<T>* GetNodeOrNull(const T data)
	{
		mData = data;
		return GetNodeOrNullRecursive(mRoot);
	}

	void DeleteNode(const T data)
	{
		if (!mRoot) // No Data
		{
			return;
		}

		if (mRoot->GetData() == data) // Delete root node
		{
			SimpleBTree<T>* virtualRoot = new SimpleBTree<T>();
			virtualRoot->MakeLeftSubTree(mRoot);

			DeleteNodeDetail(virtualRoot, mRoot, true);

			mRoot = virtualRoot->GetLeftTree();
			delete virtualRoot;
			mRoot = Rebalance(mRoot);
			return;
		}

		mData = data;
		SimpleBTree<T>* parentNode = GetParentNodeOrNullRecursive(mRoot);
		if (!parentNode) // Fail to find
		{
			return;
		}
		
		assert(parentNode);

		SimpleBTree<T>* leftChildNode = parentNode->GetLeftTree();
		if (leftChildNode && leftChildNode->GetData() == data)
		{
			DeleteNodeDetail(parentNode, leftChildNode, true);
		}


		SimpleBTree<T>* rightChildNode = parentNode->GetRightTree();
		if (rightChildNode && rightChildNode->GetData() == data)
		{
			DeleteNodeDetail(parentNode, rightChildNode, false);
		}

		parentNode = Rebalance(parentNode);
		mRoot = Rebalance(mRoot);
	}

	void SearchInOrder()
	{
		mRoot->SearchInOrderBTreeRecursive(Print);
		std::cout << std::endl;
	}

private:
	void SearchNodeAndInsertRecursive(SimpleBTree<T>* node)
	{
		static SimpleBTree<T>* pNode;

		if (!node)
		{
			SimpleBTree<T>* newNode = new SimpleBTree<T>(mData);

			if (pNode->GetData() > mData)
			{
				pNode->SetLeftTree(newNode);
			}
			else if (pNode->GetData() < mData)
			{
				pNode->SetRightTree(newNode);
			}

			return;
		}

		if (node->GetData() > mData)
		{
			pNode = node;
			SearchNodeAndInsertRecursive(node->GetLeftTree());
			node = Rebalance(node);
		}
		else if (node->GetData() < mData)
		{
			pNode = node;
			SearchNodeAndInsertRecursive(node->GetRightTree());
			node = Rebalance(node);
		}
		else
		{
			return;
		}
	}

	SimpleBTree<T>* GetNodeOrNullRecursive(SimpleBTree<T>* node) const
	{
		if (!node)
		{
			return nullptr;
		}

		if (node->GetData() == mData)
		{
			return node;
		}

		if (node->GetData() > mData)
		{
			return GetNodeOrNullRecursive(node->GetLeftTree());
		}
		else
		{
			return GetNodeOrNullRecursive(node->GetRightTree());
		}
	}

	SimpleBTree<T>* GetParentNodeOrNullRecursive(SimpleBTree<T>* node) const
	{
		if (!node)
		{
			return nullptr;
		}

		if (node->GetData() == mData)
		{
			return reinterpret_cast<SimpleBTree<T>*>(0xffffffff);
		}

		SimpleBTree<T>* parentNode;
		if (node->GetData() > mData)
		{
			parentNode = GetParentNodeOrNullRecursive(node->GetLeftTree());
		}
		else if (node->GetData() < mData)
		{
			parentNode = GetParentNodeOrNullRecursive(node->GetRightTree());
		}
		else
		{
			return nullptr;
		}

		if (parentNode == reinterpret_cast<SimpleBTree<T>*>(0xffffffff))
		{
			return node;
		}
		else
		{
			return parentNode;
		}
	}

	void DeleteNodeDetail(SimpleBTree<T>* parentNode, SimpleBTree<T>* targetNode, bool bIsLeftChild) const
	{
		if (!targetNode->GetLeftTree() && !targetNode->GetRightTree()) // No has child
		{
			delete targetNode;

			if (bIsLeftChild)
			{
				parentNode->SetLeftTree(nullptr);
			}
			else
			{
				parentNode->SetRightTree(nullptr);
			}
		}
		else if (targetNode->GetLeftTree()) // has Left child or has left-right child
		{
			SimpleBTree<T>* tParentNode = targetNode->GetLeftTree();
			SimpleBTree<T>* tMinNode = tParentNode->GetRightTree();

			if (!tMinNode)
			{
				if (bIsLeftChild)
				{
					parentNode->SetLeftTree(tParentNode);
				}
				else
				{
					parentNode->SetRightTree(tParentNode);
				}
				tParentNode->SetRightTree(targetNode->GetRightTree());
				delete targetNode;

				return;
			}

			while (tMinNode->GetRightTree())
			{
				tParentNode = tParentNode->GetRightTree();
				tMinNode = tMinNode->GetRightTree();
			}

			targetNode->SetData(tMinNode->GetData());
			delete tMinNode;
			tParentNode->SetRightTree(nullptr);
		}
		else // has Right child
		{
			SimpleBTree<T>* tParentNode = targetNode->GetRightTree();
			SimpleBTree<T>* tMaxNode = tParentNode->GetLeftTree();

			if (!tMaxNode)
			{
				if (bIsLeftChild)
				{
					parentNode->SetLeftTree(tParentNode);
				}
				else
				{
					parentNode->SetRightTree(tParentNode);
				}
				tParentNode->SetLeftTree(targetNode->GetLeftTree());
				delete targetNode;

				return;
			}

			while (tMaxNode->GetLeftTree())
			{
				tParentNode = tParentNode->GetLeftTree();
				tMaxNode = tMaxNode->GetLeftTree();
			}

			targetNode->SetData(tMaxNode->GetData());
			delete tMaxNode;
			tParentNode->SetLeftTree(nullptr);
		}
	}

	int GetHeight(SimpleBTree<T>* node) const
	{
		if (!node)
		{
			return 0;
		}

		int leftHeight = GetHeight(node->GetLeftTree());
		int rightHeight = GetHeight(node->GetRightTree());

		if (leftHeight > rightHeight)
		{
			return leftHeight + 1;
		}
		else
		{
			return rightHeight + 1;
		}
	}

	int GetHeightDiff(SimpleBTree<T>* node) const
	{
		if (!node)
		{
			return 0;
		}

		int leftSubHeight = GetHeight(node->GetLeftTree());
		int rightSubHeight = GetHeight(node->GetRightTree());

		return leftSubHeight - rightSubHeight;
	}

	SimpleBTree<T>* RotateLL(SimpleBTree<T>* node)
	{
		SimpleBTree<T>* pNode = node;
		SimpleBTree<T>* cNode = node->GetLeftTree();

		pNode->SetLeftTree(cNode->GetRightTree());
		cNode->SetRightTree(pNode);

		mData = node->GetData();
		SimpleBTree<T>* ppNode = GetParentNodeOrNullRecursive(mRoot);
		if (node == mRoot)
		{
			return cNode;
		}
		else if (ppNode->GetData() > mData)
		{
			ppNode->SetLeftTree(cNode);
		}
		else
		{
			ppNode->SetRightTree(cNode);
		}

		return cNode;
	}

	SimpleBTree<T>* RotateRR(SimpleBTree<T>* node)
	{
		SimpleBTree<T>* pNode = node;
		SimpleBTree<T>* cNode = node->GetRightTree();

		pNode->SetRightTree(cNode->GetLeftTree());
		cNode->SetLeftTree(pNode);

		mData = node->GetData();
		SimpleBTree<T>* ppNode = GetParentNodeOrNullRecursive(mRoot);
		if (node == mRoot)
		{
			return cNode;
		}
		else if (ppNode->GetData() > mData)
		{
			ppNode->SetLeftTree(cNode);
		}
		else
		{
			ppNode->SetRightTree(cNode);
		}

		return cNode;
	}

	SimpleBTree<T>* RotateLR(SimpleBTree<T>* node)
	{
		SimpleBTree<T>* pNode = node;
		SimpleBTree<T>* cNode = node->GetLeftTree();

		pNode->SetLeftTree(RotateRR(cNode));

		return RotateLL(pNode);
	}

	SimpleBTree<T>* RotateRL(SimpleBTree<T>* node)
	{
		SimpleBTree<T>* pNode = node;
		SimpleBTree<T>* cNode = node->GetRightTree();

		pNode->SetRightTree(RotateLL(cNode));

		return RotateRR(pNode);
	}
	
	SimpleBTree<T>* Rebalance(SimpleBTree<T>* node)
	{
		bool bIsRoot = false;
		if (node == mRoot)
		{
			bIsRoot = true;
		}
		int diffHeight = GetHeightDiff(node);

		if (diffHeight >= 2) // Need LL, LR rotation
		{
			if (GetHeightDiff(node->GetLeftTree()) > 0) // LL rotation
			{
				node = RotateLL(node);
			}
			else // LR Rotation
			{
				node = RotateLR(node);
			}
		}
		else if (diffHeight <= -2) // Need RR, RL rotation
		{
			if (GetHeightDiff(node->GetRightTree()) < 0) // RR rotation
			{
				node = RotateRR(node);
			} 
			else // RL rotation
			{
				node = RotateRL(node);
			}
		}

		if (bIsRoot)
		{
			mRoot = node;
		}
		return node;
	}

	SimpleBTree<T>* mRoot;
	T mData;
};