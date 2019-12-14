#include "File.h"

File::File(const char* fileName)
{
	ifstream fin;
	mData = new char[STR_LEN];

	fin.open(fileName, ios::binary);

	fin.seekg(0, ifstream::end);
	int fileSize = static_cast<int>(fin.tellg());
	fin.seekg(0, ifstream::beg);

	mData[fileSize] = '\0';
	fin.read(mData, fileSize);
}

File::~File()
{
	delete[] mData;
}

char* File::GetReadingFile() const
{
	return mData;
}

unsigned int File::GetUnsignedInt(const char* p) const
{
	const unsigned char* up;
	up = reinterpret_cast<const unsigned char*>(p);
	unsigned r = up[0];
	r |= up[1] << 8;
	r |= up[2] << 16;
	r |= up[3] << 24;
	return r;
}