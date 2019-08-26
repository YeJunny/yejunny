#define _CRT_SECURE_NO_WARNINGS

#include "Stage.h"

Stage::Stage(const char* stage, const char* image)
{
	// �� ������ �о����
	File* file = new File(stage);
	char* str = file->GetReadingFile();

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
	mDrawStage = new EDraw[length];
	memcpy(mStage, temp, length);

	delete[] temp;
	delete file;

	// �̹��� ������ �о����
	mImage = new Image(image);


	// ������� �ʱ�ȭ
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
	delete mImage;
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
	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			char data = mStage[i * mWidth + j];
			EDraw image;

			switch (data)
			{
			case ' ':
			case '.':
			{
				image = EDraw::Space;
				break;
			}
			case 'w':
			{
				image = EDraw::Wall;
				break;
			}
			case 'x':
			{
				image = EDraw::Target;
				break;
			}
			default:
			{
				image = EDraw::Space;
				break;
			}
			}

			mDrawStage[i * mWidth + j] = image;
		}
	}

	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mO[i] >= 0 && mO[i] < mWidth * mHeight)
		{
			*(mDrawStage + mO[i]) = EDraw::Box; //'O';
		}
	}

	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mQ[i] >= 0 && mQ[i] < mWidth * mHeight)
		{
			*(mDrawStage + mQ[i]) = EDraw::Goal; // 'Q';
		}
	}

	if (mP >= 0 && mP < mWidth * mHeight)
	{
		*(mDrawStage + mP) = EDraw::Player; // 'p';
	}

	unsigned int* vram = Framework::instance().videoMemory();
	unsigned int windowWidth = Framework::instance().width();
	unsigned int windowHeight = Framework::instance().height();
	const unsigned int tileSize = 32;

	for (int i = 0; i < mHeight * tileSize; i += tileSize)
	{
		for (int j = 0; j < mWidth * tileSize; j += tileSize)
		{
			EDraw drawStage = mDrawStage[(i / tileSize) * mWidth + (j / tileSize)];

			switch (drawStage)
			{
			case EDraw::Player:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = 0; dx < tileSize; dx++)
					{
						unsigned int* vramRef = &vram[(i + dy) * windowWidth + j + dx];
						*vramRef = mImage->GetData(dy * mImage->GetWidth() + dx);
					}
				}
				break;
			}
			case EDraw::Wall:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = tileSize; dx < tileSize * 2; dx++)
					{
						unsigned int* vramRef = &vram[(i + dy) * windowWidth + j + (dx - tileSize)];
						*vramRef = mImage->GetData(dy * mImage->GetWidth() + dx);
					}
				}
				break;
			}
			case EDraw::Box:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = tileSize * 2; dx < tileSize * 3; dx++)
					{
						unsigned int* vramRef = &vram[(i + dy) * windowWidth + j + (dx - tileSize * 2)];
						*vramRef = mImage->GetData(dy * mImage->GetWidth() + dx);
					}
				}
				break;
			}
			case EDraw::Goal:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = tileSize * 3; dx < tileSize * 4; dx++)
					{
						unsigned int& vramRef = vram[(i + dy) * windowWidth + j + (dx - tileSize * 3)];
						vramRef = mImage->GetData(dy * mImage->GetWidth() + dx);
					}
				}
				break;
			}
			case EDraw::Target:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = tileSize * 4; dx < tileSize * 5; dx++)
					{
						unsigned int& vramRef = vram[(i + dy) * windowWidth + j + (dx - tileSize * 4)];
						vramRef = mImage->GetData(dy * mImage->GetWidth() + dx);
					}
				}
				break;
			}
			default:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = tileSize * 5; dx < tileSize * 6; dx++)
					{
						unsigned int& vramRef = vram[(i + dy) * windowWidth + j + (dx - tileSize * 5)];
						vramRef = mImage->GetData(dy * mImage->GetWidth() + dx);
					}
				}
				break;
			}
			}
		}
	}
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