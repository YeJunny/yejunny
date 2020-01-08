#pragma once

#define STR_LEN (50)

class PersonInfo
{
public:
	PersonInfo();
	PersonInfo(int ssn, const char* name, const char* address);
	~PersonInfo();

	void Insert(int ssn, const char* name, const char* address);
	int GetSsn();
	void ShowPersonInfo();

private:
	int mSsn;
	char mName[STR_LEN];
	char mAddress[STR_LEN];
};

