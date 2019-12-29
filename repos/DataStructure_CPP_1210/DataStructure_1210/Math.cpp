#include "Math.h"

bool Math::CheckPowerOfTwo1(unsigned int number)
{
	if (number == (number & (~number + 0x1)))
	{
		return true;
	}

	return false;
}

bool Math::CheckPowerOfTwo2(unsigned int number)
{
	return !(number & (number - 0x1));
}