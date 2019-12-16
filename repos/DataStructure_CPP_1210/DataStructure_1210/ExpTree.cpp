#include "ExpTree.h"
#include "SimpleStack.h"
#include <iostream>
#include <string>

void PrintData(int data)
{
	std::cout << (char)data;
}

ExpTree::ExpTree(const char* exp)
{
	size_t expLen = strlen(exp);
	SimpleStack<SimpleBTree<int>*>* stack = new SimpleStack<SimpleBTree<int>*>();
	SimpleBTree<int>** expNodes = new SimpleBTree<int>*[expLen];
	for (size_t i = 0; i < expLen; ++i)
	{
		int data = exp[i];
		expNodes[i] = new SimpleBTree<int>(data);
		switch (data)
		{
		case '+': case '-':
		case '*': case '/':
		{
			SimpleBTree<int>* leftNode;
			SimpleBTree<int>* rightNode;
			
			stack->Pop(&rightNode);
			stack->Pop(&leftNode);

			expNodes[i]->MakeLeftSubTree(leftNode);
			expNodes[i]->MakeRightSubTree(rightNode);

			stack->Push(expNodes[i]);

			break;
		}
		default:
		{
			stack->Push(expNodes[i]);
			break;
		}
		}
	}

	stack->Pop(&mRoot);

	delete stack;
}

ExpTree::~ExpTree()
{
	mRoot->DeleteBTree();
}

void ExpTree::ShowExpPrefixOrder() const
{
	mRoot->SearchPreOrderBTree(PrintData);
}

void ExpTree::ShowExpInfixOrder() const
{
	ShowExpInfixOrderRecursive(mRoot);
}

void ExpTree::ShowExpInfixOrderRecursive(SimpleBTree<int>* node) const
{
	if (node == nullptr)
	{
		return;
	}

	if (node->GetLeftTree())
	{
		std::cout << "(";
	}

	ShowExpInfixOrderRecursive(node->GetLeftTree());
	PrintData(node->GetData());
	ShowExpInfixOrderRecursive(node->GetRightTree());

	if (node->GetLeftTree())
	{
		std::cout << ")";
	}
}

void ExpTree::ShowExpPostfixOrder() const
{
	mRoot->SearchPostOrderBTree(PrintData);
}

float ExpTree::CalulateExp() const
{
	return CalulateExpRecursive(mRoot);
}

float ExpTree::CalulateExpRecursive(SimpleBTree<int>* node) const
{
	if (node == nullptr)
	{
		return 0;
	}

	float leftResult = CalulateExpRecursive(node->GetLeftTree());
	float rightResult = CalulateExpRecursive(node->GetRightTree());
	char data = node->GetData();
	switch (data)
	{
	case '+': case '-':
	case '*': case '/':
	{
		float operand1 = node->GetLeftTree()->GetData() - '0';
		float operand2 = node->GetRightTree()->GetData() - '0';

		if (operand1 < 0)
		{
			operand1 = leftResult;
		}
		if (operand2 < 0)
		{
			operand2 = rightResult;
		}

		float result = 0;
		if (data == '+')
		{
			result = operand1 + operand2;
		}
		else if (data == '-')
		{
			result = operand1 - operand2;
		}
		else if (data == '*')
		{
			result = operand1 * operand2;
		}
		else if (data == '/')
		{
			result = operand1 / operand2;
		}

		return result;
	}
	default:
	{
		return 0;
	}
	}
}