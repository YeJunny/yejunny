#pragma once

#include <iostream>

class Vector
{
public:
	Vector() = default;

	Vector(float x, float y)
		: mX(x)
		, mY(y)
	{
	}

	void Print() const
	{
		std::cout << mX << ", " << mY << std::endl;
	}

	void Add(const Vector* other)
	{
		mX += other->mX;
		mY += other->mY;
	}

private:
	float mX;
	float mY;
};
