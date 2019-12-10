#include <iostream>
#include "LinkedList.h"
#include "SimpleBTree.h"
using namespace std;

void PrintData(int data)
{
	cout << data << endl;
}

int main()
{
	LinkedList* linkedList = new LinkedList();
	linkedList->Insert(10);
	linkedList->Insert(100);
	linkedList->Insert(1000);
	int data;
	while (linkedList->Next(&data))
	{
		cout << data << endl;
	}
	delete linkedList;


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
