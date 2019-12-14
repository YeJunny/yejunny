#include "Vector2.h"

Vector2::Vector2()
	: X(0.)
	, Y(0.)
{
}

Vector2::Vector2(int x, int y)
{
	X = static_cast<double>(x);
	Y = static_cast<double>(y);
}

Vector2::Vector2(double x, double y)
	: X(x)
	, Y(y)
{
}
