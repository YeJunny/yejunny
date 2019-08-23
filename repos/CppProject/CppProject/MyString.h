//#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <string>

class MyString
{
public:
	MyString(MyString&& other);
	MyString(MyString& other);
	MyString(const char* str);
	~MyString();
	MyString& operator=(MyString&& other);

private:
	char* mString;
	int mSize;
};
