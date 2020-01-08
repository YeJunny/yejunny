#pragma once

class PersonInfo;

typedef int Key;
typedef PersonInfo* Value;

struct Slot
{
	Key Key;
	Value Value;
};