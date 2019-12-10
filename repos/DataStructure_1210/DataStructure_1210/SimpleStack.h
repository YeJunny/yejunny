#pragma once
#include "LinkedList.h"

typedef int DataType;

class SimpleStack
{
public:
	SimpleStack();
	~SimpleStack();

	void Push(DataType data);
	DataType Pop() const;
	void DeleteStack() const;

private:
	LinkedList* mLinkedList;
};

