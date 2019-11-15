#include "MyString.h"

MyString::MyString(MyString&& other)
	: mString(other.mString)
	, mSize(other.mSize)
{
	other.mString = nullptr;
	other.mSize = 0;
}

MyString::MyString(MyString& other)
	: mSize(other.mSize)
{
	mString = new char[mSize];
	memcpy(mString, other.mString, mSize);
}

MyString::MyString(const char* str)
	: mSize(strlen(str) + 1)
{
	mString = new char[mSize];
	memcpy(mString, str, mSize);
}

MyString::~MyString()
{
	delete[] mString;
}

MyString& MyString::operator=(MyString&& other)
{
	if (this != &other)
	{
		delete[] mString;

		mString = other.mString;
		mSize = other.mSize;

		other.mString = nullptr;
		other.mSize = 0;
	}
	return *this;
}
