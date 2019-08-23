#pragma once
#include <string>

using namespace std;

class Cat
{
public:
	Cat(const char* name);
	const char* GetName() const;
	~Cat();
	
private:
	char* mName;
};