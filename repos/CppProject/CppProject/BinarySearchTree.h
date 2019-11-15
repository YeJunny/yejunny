#pragma once

#include <iostream>

using namespace std;

struct Node
{
	int Value;
	Node* Left;
	Node* Right;
};

class BST
{
public:
	Node* Insert(Node* node, int value);
	void Inorder(Node* node);
	Node* Search(Node* node, int value);
	void Remove(Node* node);
};