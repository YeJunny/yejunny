#pragma once

typedef int DataType;

struct Node;

class LinkedList
{
public:
	LinkedList();
	~LinkedList();

	void Insert(DataType data);
	bool First(DataType* outData = nullptr);
	bool Next(DataType* outData = nullptr);
	bool Remove(DataType* outData = nullptr);
	void SetCurrentPositionHead();
	void SetCurrentPositionTail();
	DataType GetData() const;

private:
	Node* mHead;
	Node* mCurrent;
	Node* mTail;
};

