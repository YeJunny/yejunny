#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "MergeSort.h"
#include "SelectionSort.h"
#include "QuickSort.h"
using namespace std;

int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int min(int a, int b)
{
	if (a > b)
	{
		return b;
	}
	else
	{
		return a;
	}
}

int gcd(int num1, int num2)
{
	int a = max(num1, num2);
	int b = min(num1, num2);

	while (b > 0)
	{
		int rem = a % b;
		a = b;
		b = rem;
	}

	return a;
}

class Parent
{
public:
	Parent() : mX(1) { cout << "Parent Class Created!\n"; }
	~Parent() { cout << "Parent Class Deleted!\n"; }
	int GetX() const { return mX; }

private:
	int mX;
};

class Child : public Parent
{
public:
	Child(int x, int y) : mY(y) { cout << "Child Class Created!\n"; }
	~Child() { cout << "Child Class Deleted!\n"; }
	int GetY() const { return mY; }

private:
	int mY;
};

int main()
{
	Child* child = new Child(1, 2);

	delete child;

	return 0;
}
