#pragma once

typedef int DataType;

struct Node;

class LinkedList
{
public:
	LinkedList();
	~LinkedList();

	void Insert(DataType data);
	bool Next(DataType* outData = nullptr);
	void Remove();

private:
	Node* mHead;
	Node* mCurrent;
};

