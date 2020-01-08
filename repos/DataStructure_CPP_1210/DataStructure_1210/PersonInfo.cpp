#include "PersonInfo.h"
#include <iostream>

PersonInfo::PersonInfo()
{
}

PersonInfo::PersonInfo(int ssn, const char* name, const char* address)
	: mSsn(ssn)
{
	strncpy_s(mName, name, strlen(name));
	strncpy_s(mAddress, address, strlen(address));
}

PersonInfo::~PersonInfo()
{
}

void PersonInfo::Insert(int ssn, const char* name, const char* address)
{
	mSsn = ssn;
	strncpy_s(mName, name, strlen(name));
	strncpy_s(mAddress, address, strlen(address));
}

int PersonInfo::GetSsn()
{
	return mSsn;
}

void PersonInfo::ShowPersonInfo()
{
	std::cout << "SSN : " << mSsn << std::endl;
	std::cout << "Name : " << mName << std::endl;
	std::cout << "Address : " << mAddress << std::endl;
}
