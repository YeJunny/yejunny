#include "Map.h"
#include "GLManager.h"
#include "glfw-3.3.2.bin.WIN64/include/GLFW/glfw3.h"
#include <iostream>
#include <fstream>

#pragma warning(disable:4996)

Map::Map(const char* mapFileName)
	: mNumO(0.0f)
	, mNumP(0.0f)
	, mbP(false)
	, mbO(false)
	, mPrevPlayerIndex(-1)
	, mPlayerIndex(-1)
{
	// ===========================
	// Read Map
	
	char* buffer = new char[512];

	std::ifstream fin;
	fin.open(mapFileName, std::ios::binary);
	fin.seekg(0, std::ios::end);
	int fileSize = fin.tellg();
	fin.seekg(0, std::ios::beg);
	fin.read(buffer, fileSize);
	fin.close();
	buffer[fileSize] = 0;

	char* token = strtok(buffer, "\n\r");
	mWidth = atoi(token);

	token = strtok(NULL, "\n\r");
	mHeight = atoi(token);

	char* mapData = new char[128];
	token = strtok(NULL, "\n\r");
	strcpy(mapData, token);
	token = strtok(NULL, "\n\r");

	while (token != NULL)
	{
		strcat(mapData, token);
		token = strtok(NULL, "\n\r");
	}

	size_t length = strlen(mapData);
	mRenderConsoleArray = new char[length + 1]; // Render Console
	mOriginalMap = new EObject[length];
	mObjectMap = new EObject[length];

	delete[] buffer;


	// ============================
	// Init Member Variable

	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			int index = y * mWidth + x;
			if (mapData[index] == 'O')
			{
				++mBoxCount;
			}
		}
	}

	mFilled = new int[mBoxCount];
	for (int i = 0; i < mBoxCount; i++)
	{
		mFilled[i] = -1;
	}

	mBox = new int[mBoxCount];
	mPrevBox = new int[mBoxCount];
	int boxCount = 0;
	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			int index = y * mWidth + x;

			if (mapData[index] == 'w')
			{
				mOriginalMap[index] = EObject::Wall;
			}
			else if (mapData[index] == '.')
			{
				mOriginalMap[index] = EObject::Background;
			}
			else if (mapData[index] == 'x')
			{
				mOriginalMap[index] = EObject::Target;
			}
			else if (mapData[index] == 'O')
			{
				mBox[boxCount] = index;
				mPrevBox[boxCount] = index;
				mOriginalMap[index] = EObject::Other;
				++boxCount;
			}
			else if (mapData[index] == 'p')
			{
				mPlayerIndex = index;
				mPrevPlayerIndex = index;
				mOriginalMap[index] = EObject::Other;
			}
		}
	}

	delete[] mapData;
}

Map::~Map()
{
	delete[] mBox;
	delete[] mPrevBox;
	delete[] mFilled;
	delete[] mObjectMap;
	delete[] mOriginalMap;
}

void Map::Update(GLManager* glManager)
{
	if (glManager->GetIsRenderMovingPlayer())
	{
		return;
	}


	// =====================================
	// Calculate diff

	int diff = 0;
	
	if (glManager->GetKeyDown(GLFW_KEY_UP))
	{
		diff = -mWidth;
	}
	else if (glManager->GetKeyDown(GLFW_KEY_DOWN))
	{
		diff = mWidth;
	}
	else if (glManager->GetKeyDown(GLFW_KEY_LEFT))
	{
		diff = -1;
	}
	else if (glManager->GetKeyDown(GLFW_KEY_RIGHT))
	{
		diff = 1;
	}


	// =======================================
	// Memory Previous Position

	mPrevPlayerIndex = mPlayerIndex;
	for (int i = 0; i < mBoxCount; i++)
	{
		mPrevBox[i] = mBox[i];
	}


	// =============================
	// Calcute Map

	mPlayerIndex += diff;

	for (int i = 0; i < mBoxCount; i++)
	{
		if (mFilled[i] == mPlayerIndex) // Q�� ���� �ѹ��� �ִ��� Ȯ��
		{
			int qIndex = mPlayerIndex + diff;	// p�� �� ��ŭ

			// Q�� Q�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mBoxCount; j++)
			{
				if (mFilled[j] == qIndex)
				{
					mPlayerIndex -= diff;
					return;
				}
			}

			// Q�� O�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mBoxCount; j++)
			{
				if (mBox[j] == qIndex)
				{
					mPlayerIndex -= diff;
					return;
				}
			}

			// Q ��������,
			for (int j = 0; j < mWidth * mHeight; j++)
			{
				if (*(mOriginalMap + j) == EObject::Wall && j == qIndex)	// Q�� ���� �浹�ߴ��� Ȯ��
				{
					qIndex -= diff;
					mPlayerIndex -= diff;
					return;
				}

				if (*(mOriginalMap + j) == EObject::Target && j == qIndex)	// Q�� x�� �浹�ߴ��� Ȯ��
				{
					for (int k = 0; k < mBoxCount; k++)
					{
						if (mBox[k] == mFilled[i])
						{
							mBox[k] += diff;
							mFilled[i] += diff;
							return;
						}
					}
				}
			}

			// ��ĭ�� ���
			for (int j = 0; j < mBoxCount; j++)
			{
				if (mBox[j] == mPlayerIndex)
				{
					mBox[j] += diff;
					mFilled[i] = -1;
					return;
				}
			}
		}
	}

	for (int i = 0; i < mBoxCount; i++)
	{
		if (mBox[i] == mPlayerIndex)	// O�� ���� �ѹ��� �ִ��� Ȯ��
		{
			int oIndex = mPlayerIndex + diff;	// p�� �� ��ŭ

			// O ��������,
			for (int j = 0; j < mWidth * mHeight; j++)
			{
				if (*(mOriginalMap + j) == EObject::Wall && j == oIndex)	// O�� ���� �浹�ߴ��� Ȯ��
				{
					oIndex -= diff;
					mPlayerIndex -= diff;
					return;
				}

				if (*(mOriginalMap + j) == EObject::Target && j == oIndex)	// O�� x�� �浹�ߴ��� Ȯ��
				{
					for (int k = 0; k < mBoxCount; k++)
					{
						if (mBox[k] == oIndex)
						{
							mPlayerIndex -= diff;
							return;
						}
					}
					mBox[i] += diff;

					for (int k = 0; k < mBoxCount; k++)
					{
						if (mFilled[k] < 0)
						{
							mFilled[k] = mBox[i];
							return;
						}
					}
				}
			}

			// O�� Q�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mBoxCount; j++)
			{
				if (mFilled[j] == oIndex)
				{
					mPlayerIndex -= diff;
					return;
				}
			}

			// O�� O�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mBoxCount; j++)
			{
				if (mBox[j] == oIndex)
				{
					mPlayerIndex -= diff;
					return;
				}
			}

			// ��ĭ�� ���
			mBox[i] += diff;
		}
	}

	for (int i = 0; i < mWidth * mHeight; i++) // ���� ��Ҵ��� Ȯ��
	{
		if (*(mOriginalMap + i) == EObject::Wall && i == mPlayerIndex)
		{
			mPlayerIndex -= diff;
			return;
		}
	}
}

void Map::UpdateMap()
{
	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			int index = y * mWidth + x;

			mObjectMap[index] = mOriginalMap[index];
			
			if (mPrevPlayerIndex == index)
			{
				mObjectMap[index] = EObject::PlayerFrom;
			}
			else if (mPlayerIndex == index)
			{
				mObjectMap[index] = EObject::PlayerTo;
			}
		}
	}

	if (mPrevPlayerIndex == mPlayerIndex)
	{
		mObjectMap[mPlayerIndex] = EObject::PlayerInPlace;
	}

	for (int i = 0; i < mBoxCount; ++i)
	{
		mObjectMap[mBox[i]] = EObject::Box;
	}

	for (int i = 0; i < mBoxCount; ++i)
	{
		if (mFilled[i] > 0)
		{
			mObjectMap[mFilled[i]] = EObject::Filled;
		}
	}
}

void Map::RenderConsole() const
{
	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			int index = y * mWidth + x;
			EObject object = mObjectMap[index];

			switch (object)
			{
			case EObject::Background:
			case EObject::Other:
			{
				std::cout << ' ';
				break;
			}
			case EObject::Wall:
			{
				std::cout << 'w';
				break;
			}
			case EObject::Box:
			{
				std::cout << 'O';
				break;
			}
			case EObject::Filled:
			{
				std::cout << 'Q';
				break;
			}
			case EObject::Target:
			{
				std::cout << 'x';
				break;
			}
			case EObject::PlayerTo:
			case EObject::PlayerInPlace:
			{
				std::cout << 'p';
				break;
			}
			}
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

bool Map::IsClear() const
{
	int count = 0;
	for (int i = 0; i < mBoxCount; i++)
	{
		if (mFilled[i] > 0)
		{
			++count;
		}
	}

	if (count == mBoxCount)
	{
		return true;
	}

	return false;
}
