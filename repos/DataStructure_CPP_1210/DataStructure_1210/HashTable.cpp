#include "HashTable.h"

HashTable::HashTable(HashFunction* hashFunc)
	: mHashFunc(hashFunc)
{
	mLinkedList = new LinkedList<Slot*>[MAX_TABLE];
}

HashTable::~HashTable()
{
	Slot* slot;

	for (int i = 0; i < MAX_TABLE; ++i)
	{
		if (mLinkedList[i].First(&slot))
		{
			delete slot;
		}
		while (mLinkedList[i].Next(&slot))
		{
			if (slot)
			{
				delete slot;
			}
		}
	}
	delete[] mLinkedList;
}

void HashTable::Insert(Key key, Value value)
{
	Slot* slot = new Slot();
	slot->Key = key;
	slot->Value = value;

	int index = mHashFunc(key);

	if (!mLinkedList[index].GetData())
	{
		mLinkedList[index].Insert(slot);
	}
}

Value HashTable::Delete(Key key)
{
	Slot* slot;

	int index = mHashFunc(key);

	if (mLinkedList[index].First(&slot))
	{
		if (slot->Key == key)
		{
			Value value = slot->Value;
			delete slot;
			mLinkedList[index].Remove();
			return value;
		}
		else
		{
			while (mLinkedList[index].Next(&slot))
			{
				if (slot->Key == key)
				{
					Value value = slot->Value;
					delete slot;
					mLinkedList[index].Remove();
					return value;
				}
			}
		}
	}

	return nullptr;
}

Value HashTable::GetValueOrNull(Key key)
{
	Slot* slot;

	int index = mHashFunc(key);
	
	if (mLinkedList[index].First(&slot))
	{
		if (slot->Key == key)
		{
			return slot->Value;
		}
		else
		{
			while (mLinkedList[index].Next(&slot))
			{
				if (slot->Key == key)
				{
					slot->Value = mLinkedList[index].GetData()->Value;
					return slot->Value;
				}
			}
		}
	}

	return nullptr;
}