#pragma once

#define MAX_TABLE (500)

#include "LinkedList.h"

template <typename TKey, typename KValue>
struct Slot
{
	TKey Key;
	KValue Value;
};

template <typename TKey, typename KValue>
class HashTable
{
public:
	typedef int HashFunction(TKey key);

	HashTable(HashFunction* hashFunc)
		: mHashFunc(hashFunc)
	{
		mLinkedList = new LinkedList<Slot<TKey, KValue>*>[MAX_TABLE];
	}

	~HashTable()
	{
		Slot<TKey, KValue>* slot;

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

	void Insert(TKey key, KValue value)
	{
		Slot<TKey, KValue>* slot = new Slot<TKey, KValue>();
		slot->Key = key;
		slot->Value = value;

		int index = mHashFunc(key);

		if (!mLinkedList[index].GetData())
		{
			mLinkedList[index].Insert(slot);
		}
		else
		{
			Slot<TKey, KValue>* outSlot;
			if (mLinkedList[index].First(&outSlot))
			{
				if (outSlot->Key == key)
				{
					std::cout << "Same Key. Input Error" << std::endl;
					return;
				}

				while (mLinkedList[index].Next(&outSlot))
				{
					if (outSlot->Key == key)
					{
						std::cout << "Same Key. Input Error" << std::endl;
						return;
					}
				}

				mLinkedList[index].Insert(slot);
			}
		}
	}

	KValue Delete(TKey key)
	{
		Slot<TKey, KValue>* slot;

		int index = mHashFunc(key);

		if (mLinkedList[index].First(&slot))
		{
			if (slot->Key == key)
			{
				KValue value = slot->Value;
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
						KValue value = slot->Value;
						delete slot;
						mLinkedList[index].Remove();
						return value;
					}
				}
			}
		}

		return nullptr;
	}

	KValue GetValueOrNull(TKey key)
	{
		Slot<TKey, KValue>* slot;

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

private:
	LinkedList<Slot<TKey, KValue>*>* mLinkedList;
	HashFunction* mHashFunc;
};

