#pragma once

#define STR_LEN (1024 * 30)
#include <fstream>

using namespace std;

class File
{
public:
	File(const char* fileName);
	~File();
	char* GetReadingFile() const;
	unsigned int GetUnsignedInt(const char* p);

private:
	char* mData;
};