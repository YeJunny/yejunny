#define _CRT_SECURE_NO_WARNINGS

#include "Cat.h"

Cat::Cat(const char* name)
{
	size_t size = strlen(name) + 1;
	mName = new char[size];
	strcpy(mName, name);
}

const char* Cat::GetName() const
{
	return mName;
}

Cat::~Cat()
{
	delete[] mName;
}