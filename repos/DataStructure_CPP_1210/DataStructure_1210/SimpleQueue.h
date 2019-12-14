#pragma once
#include "LinkedList.h"

typedef int DataType;

class SimpleQueue
{
public:
	SimpleQueue();
	~SimpleQueue();

	void Push(DataType data) const;
	bool Pop(DataType* outData = nullptr) const;

private:
	LinkedList* mLinkedList;
};

