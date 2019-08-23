#define _CRT_SECURE_NO_WARNINGS

#include "Stage.h"

Stage::Stage()
{
	ifstream fin;
	char* str = new char[STR_LEN];

	fin.open("stageData.txt", ios::binary);

	fin.seekg(0, ifstream::end);
	int fileSize = static_cast<int>(fin.tellg());
	fin.seekg(0, ifstream::beg);

	fin.read(str, fileSize);
	str[fileSize] = 0;

	char* token = strtok(str, "\n\r");
	mWidth = atoi(token);

	token = strtok(nullptr, "\n\r");
	mHeight = atoi(token);

	char* temp = new char[STR_LEN];
	token = strtok(nullptr, "\n\r");
	strcpy(temp, token);
	token = strtok(nullptr, "\n\r");

	while (token != nullptr)
	{
		strcat(temp, token);
		token = strtok(nullptr, "\n\r");
	}

	size_t length = strlen(temp);
	mStage = new char[length + 1];
	mDrawStage = new int[length];
	memcpy(mStage, temp, length);

	delete[] str;
	delete[] temp;
	fin.close();

	int count = 0;
	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			if (mStage[i * mWidth + j] == 'O')
			{
				++count;
			}
		}
	}
	mNumOfBox = count;

	// Goal �ʱ�ȭ
	mQ = new int[mNumOfBox];
	for (int i = 0; i < mNumOfBox; i++)
	{
		mQ[i] = -1;
	}

	mO = new int[mNumOfBox];
	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			if (mStage[i * mWidth + j] == 'p')
			{
				mP = j + mWidth * i;
				mStage[i * mWidth + j] = ' ';
			}
		}
	}

	int k = 0;
	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			if (mStage[i * mWidth + j] == 'O')
			{
				mStage[i * mWidth + j] = ' ';
				mO[k++] = j + mWidth * i;
			}
		}
	}
}

Stage::~Stage()
{
	delete[] mO;
	delete[] mQ;
	delete[] mStage;
	delete[] mDrawStage;
}

int Stage::Input()
{
	char input;
	cin >> input;

	int move;

	switch (input)
	{
	case 'A':
	case 'a':
	{
		move = -1;
		break;
	}
	case 'D':
	case 'd':
	{
		move = +1;
		break;
	}
	case 'W':
	case 'w':
	{
		move = -mWidth;
		break;
	}
	case 'S':
	case 's':
	{
		move = +mWidth;
		break;
	}
	default:
	{
		move = 0;
		break;
	}
	}

	mP += move;
	return move;
}

void Stage::Update(int move)
{
	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mQ[i] == mP) // Q�� ���� �ѹ��� �ִ��� Ȯ��
		{
			int indexOfq = mP + move;	// p�� �� ��ŭ

			// Q�� Q�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mNumOfBox; j++)
			{
				if (mQ[j] == indexOfq)
				{
					mP -= move;
					return;
				}
			}

			// Q�� O�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mNumOfBox; j++)
			{
				if (mO[j] == indexOfq)
				{
					mP -= move;
					return;
				}
			}

			// Q ��������,
			for (int j = 0; j < mWidth * mHeight; j++)
			{
				if (*(mStage + j) == 'w' && j == indexOfq)	// Q�� ���� �浹�ߴ��� Ȯ��
				{
					indexOfq -= move;
					mP -= move;
					return;
				}

				if (*(mStage + j) == 'x' && j == indexOfq)	// Q�� x�� �浹�ߴ��� Ȯ��
				{
					for (int k = 0; k < mNumOfBox; k++)
					{
						if (mO[k] == mQ[i])
						{
							mO[k] += move;
							mQ[i] += move;
							return;
						}
					}
				}
			}

			// ��ĭ�� ���
			for (int j = 0; j < mNumOfBox; j++)
			{
				if (mO[j] == mP)
				{
					mO[j] += move;
					mQ[i] = -1;
					return;
				}
			}
		}
	}

	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mO[i] == mP)	// O�� ���� �ѹ��� �ִ��� Ȯ��
		{
			int indexOfo = mP + move;	// p�� �� ��ŭ

			// O ��������,
			for (int j = 0; j < mWidth * mHeight; j++)
			{
				if (*(mStage + j) == 'w' && j == indexOfo)	// O�� ���� �浹�ߴ��� Ȯ��
				{
					indexOfo -= move;
					mP -= move;
					return;
				}

				if (*(mStage + j) == 'x' && j == indexOfo)	// O�� x�� �浹�ߴ��� Ȯ��
				{
					mO[i] += move;

					for (int k = 0; k < mNumOfBox; k++)
					{
						if (mQ[k] < 0)
						{
							mQ[k] = mO[i];
							return;
						}
					}
				}
			}

			// O�� Q�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mNumOfBox; j++)
			{
				if (mQ[j] == indexOfo)
				{
					mP -= move;
					return;
				}
			}

			// O�� O�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mNumOfBox; j++)
			{
				if (mO[j] == indexOfo)
				{
					mP -= move;
					return;
				}
			}

			// ��ĭ�� ���
			mO[i] += move;
		}
	}

	for (int i = 0; i < mWidth * mHeight; i++) // ���� ��Ҵ��� Ȯ��
	{
		if (*(mStage + i) == 'w' && i == mP)
		{
			mP -= move;
			return;
		}
	}
}

void Stage::Draw() const
{
	unsigned int* vram = Framework::instance().videoMemory();
	unsigned int windowWidth = Framework::instance().width();

	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			char data = mStage[i * mWidth + j];
			unsigned int color;

			switch (data)
			{
			case ' ':
			case '.':
			{
				color = 0x000000;
				break;
			}
			case 'w':
			{
				color = 0xffffff;
				break;
			}
			case 'x':
			{
				color = 0x0000ff;
				break;
			}
			default:
			{
				color = 0x000000;
				break;
			}
			}

			mDrawStage[i * mWidth + j] = color;
		}
	}

	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mO[i] >= 0 && mO[i] < mWidth * mHeight)
		{
			*(mDrawStage + mO[i]) = 0xff0000; //'O';
		}
	}

	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mQ[i] >= 0 && mQ[i] < mWidth * mHeight)
		{
			*(mDrawStage + mQ[i]) = 0xff00ff; // 'Q';
		}
	}

	if (mP >= 0 && mP < mWidth * mHeight)
	{
		*(mDrawStage + mP) = 0x00ff00; // 'p';
	}

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			for (int i = 0; i < 16; ++i)
			{
				for (int j = 0; j < 16; ++j)
				{
					vram[(y * 16 + i) * windowWidth + (x * 16 + j)] = mDrawStage[y * mWidth + x];
				}
			}
		}

		cout << endl;
	}

	cout << endl;
}

bool Stage::IsClear() const
{
	int count = 0;
	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mQ[i] > 0)
		{
			++count;
		}
	}

	if (count == mNumOfBox)
	{
		return true;
	}

	return false;
}