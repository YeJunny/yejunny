#pragma once

#include "SimpleBTree.h"

class ExpTree
{
public:
	ExpTree(const char* exp);
	~ExpTree();

	void ShowExpPrefixOrder() const;
	void ShowExpInfixOrder() const;
	void ShowExpPostfixOrder() const;
	float CalulateExp() const;

private:
	void ShowExpInfixOrderRecursive(SimpleBTree<int>* node) const;
	float CalulateExpRecursive(SimpleBTree<int>* node) const;

	SimpleBTree<int>* mRoot;
};

