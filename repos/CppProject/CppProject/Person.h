#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "Pet.h"

class Pet;

class Person
{
public:
	void SetPet(const std::shared_ptr<Pet>& pet)
	{
		mPet = pet;
	}

	Person(const char* name)
		: mName(name)
	{
	}
	Person() = delete;
	~Person()
	{
		std::cout << "Person destroyed..." << std::endl;
	}

private:
	std::string mName;
	std::shared_ptr<Pet> mPet;
};