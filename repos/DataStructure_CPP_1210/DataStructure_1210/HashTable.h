#pragma once

#define MAX_TABLE (100)

#include "LinkedList.h"
#include "Slot.h"

typedef int HashFunction(Key key);

class HashTable
{
public:
	HashTable(HashFunction* hashFunc);
	~HashTable();

	void Insert(Key key, Value value);
	Value Delete(Key key);
	Value GetValueOrNull(Key key);

private:
	LinkedList<Slot*>* mLinkedList;
	HashFunction* mHashFunc;
};

