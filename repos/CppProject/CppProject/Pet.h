#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "Person.h"

class Person;

class Pet
{
public:
	void SetOwner(const std::shared_ptr<Person>& owner)
	{
		mOwner = owner;
	}

	Pet(const char* name)
		: mName(name)
	{
	}

	Pet() = delete;

	~Pet()
	{
		std::cout << "Pet destroyed...." << std::endl;
	}

private:
	std::string mName;
	std::weak_ptr<Person> mOwner;
};