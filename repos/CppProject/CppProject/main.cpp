//#define _CRT_SECURE_NO_WARNINGS
#include "BinarySearchTree.h"

using namespace std;

int main()
{
	if (0b010)
	{
		cout << "0b010" << endl;
	}

	BST bst;
	Node* root = nullptr;
	root = bst.Insert(root, 5);
	bst.Insert(root, 4);
	bst.Insert(root, 1);
	bst.Insert(root, 9);
	bst.Insert(root, 2);

	bst.Inorder(root);

	Node* delNode = bst.Search(root, 1);
	bst.Remove(delNode);

	bst.Inorder(root);

	return 0;
}