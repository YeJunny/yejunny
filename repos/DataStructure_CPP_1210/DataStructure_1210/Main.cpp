#include <iostream>
#include "ConvertInToPost.h"
#include "SimpleBTree.h"
using namespace std;

void PrintData(int data)
{
	cout << data << endl;
}

int main()
{
	char* postExp = ConvertInToPost::CovertInfixToPostfix("1+2*3");
	cout << postExp << endl;

	/*SimpleBTree* bTreeRoot = new SimpleBTree(10);
	SimpleBTree* bTreeLeft = new SimpleBTree(12);
	SimpleBTree* bTreeRight = new SimpleBTree(14);

	bTreeRoot->MakeLeftSubTree(bTreeLeft);
	bTreeRoot->MakeRightSubTree(bTreeRight);

	SimpleBTree* bTreeLeftNode = new SimpleBTree(20);
	SimpleBTree* bTreeRightNode = new SimpleBTree(23);
	bTreeRight->MakeLeftSubTree(bTreeLeftNode);
	bTreeRight->MakeRightSubTree(bTreeRightNode);

	bTreeRoot->SearchPostOrderBTree(PrintData);

	bTreeRoot->DeleteBTree();*/
}
