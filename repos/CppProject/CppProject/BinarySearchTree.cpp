#include "BinarySearchTree.h"

Node* BST::Insert(Node* node, int value)
{
	if (node == nullptr)
	{
		node = new Node();
		node->Value = value;
		node->Left = nullptr;
		node->Right = nullptr;
		return node;
	}

	if (node->Value > value)
	{
		node->Left = Insert(node->Left, value);
	}
	else if (node->Value < value)
	{
		node->Right = Insert(node->Right, value);
	}

	return node;
}

void BST::Inorder(Node* node)
{
	if (node == nullptr)
	{
		return;
	}

	Inorder(node->Left);
	cout << node->Value << endl;
	Inorder(node->Right);
}

Node* BST::Search(Node* node, int value)
{
	if (node->Value == value)
	{
		return node;
	}

	if (node->Left != nullptr)
	{
		return Search(node->Left, value);
	}

	if (node->Right != nullptr)
	{
		return Search(node->Right, value);
	}

	return node;
}

void BST::Remove(Node* node)
{
	Remove(node->Left);
	if (node->Left != nullptr)
	{
		delete node->Left;
	}

	Remove(node->Right);
	if (node->Right != nullptr)
	{
		delete node->Right;
	}

	if (node != nullptr)
	{
		delete node;
	}
}