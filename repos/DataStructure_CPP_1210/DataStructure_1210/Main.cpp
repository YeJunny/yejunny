#include <iostream>
#include "HashTable.h"
#include "PersonInfo.h"
using namespace std;

int GetHashValue(int value)
{
	return value % 100;
}

int main()
{
	HashTable* hashTable = new HashTable(GetHashValue);

	PersonInfo* person = new PersonInfo[3]();
	person[0].Insert(123, "JK", "Address1");
	person[1].Insert(456, "YJK", "Address2");
	person[2].Insert(789, "BJK", "Address3");

	hashTable->Insert(302, &person[0]);
	hashTable->Insert(202, &person[1]);
	hashTable->Insert(205, &person[2]);

	PersonInfo* personInfo = hashTable->GetValueOrNull(302);
	if (personInfo)
	{
		personInfo->ShowPersonInfo();
	}

	delete[] person;
	delete hashTable;
}
