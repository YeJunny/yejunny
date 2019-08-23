#include "Stage.h"

Stage::Stage(int width, int height, char* stage)
	: mWidth(width)
	, mHeight(height)
	, mP(0)
{
	size_t length = strlen(stage);
	mStage = new char[length + 1];
	mDrawStage = new char[length + 1];
	memcpy(mStage, stage, length);

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
}

int Stage::Input()
{
	char input = 0;
	while (true)
	{
		std::cout << "Input: ";
		std::cin >> input;

		if (input == 'A' || input == 'D' || input == 'a' || input == 'd' ||
			input == 'W' || input == 'S' || input == 'w' || input == 's')
		{
			break;
		}

		std::cout << "Invaild the input. Re ";
	}

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
			mDrawStage[i * mWidth + j] = mStage[i * mWidth + j];
		}
	}

	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mO[i] >= 0 && mO[i] < mWidth * mHeight)
		{
			*(mDrawStage + mO[i]) = 'O'; //'O';
		}
	}

	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mQ[i] >= 0 && mQ[i] < mWidth * mHeight)
		{
			*(mDrawStage + mQ[i]) = 'Q'; // 'Q';
		}
	}

	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mP >= 0 && mP < mWidth * mHeight)
		{
			*(mDrawStage + mP) = 'p'; // 'p';
		}
	}

	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			std::cout << mDrawStage[i * mWidth + j];
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
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