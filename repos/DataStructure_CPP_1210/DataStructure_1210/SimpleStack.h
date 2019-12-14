#pragma once
#include "LinkedList.h"

typedef int DataType;

class SimpleStack
{
public:
	SimpleStack();
	~SimpleStack();

	void Push(DataType data) const;
	bool Pop(DataType* outData = nullptr) const;
	DataType View() const;

private:
	LinkedList* mLinkedList;
};

