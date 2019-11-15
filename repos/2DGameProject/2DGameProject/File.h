#pragma once

#define STR_LEN (400000)
#include <fstream>

using namespace std;

class File
{
public:
	File(const char* fileName);
	~File();
	char* GetReadingFile() const;
	unsigned int GetUnsignedInt(const char* p) const;

private:
	char* mData;
};