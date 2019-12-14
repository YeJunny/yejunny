#define _CRT_SECURE_NO_WARNINGS

#include "File.h"
#include "GameLib/Framework.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Manager.h"
#include "Image.h"
#include "Stage.h"

using namespace GameLib;
using namespace std;

Stage::Stage(const char* stage, const char* image)
	: mNumO(0.)
	, mNumP(0.)
	, mbP(false)
	, mbO(false)
{
	// �� ������ �о����
	File* file = new File(stage);
	char* str = file->GetReadingFile();

	char* token = strtok(str, "\n\r");
	mWidth = atoi(token);

	token = strtok(NULL, "\n\r");
	mHeight = atoi(token);

	char* temp = new char[64];
	token = strtok(NULL, "\n\r");
	strcpy(temp, token);
	token = strtok(NULL, "\n\r");

	while (token != NULL)
	{
		strcat(temp, token);
		token = strtok(NULL, "\n\r");
	}

	size_t length = strlen(temp);
	mStage = new char[length + 1];
	mFlagStage = new EFlag[length];
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
	mBoxCount = count;

	mQ = new int[mBoxCount];
	for (int i = 0; i < mBoxCount; i++)
	{
		mQ[i] = -1;
	}

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

	mO = new int[mBoxCount];
	mPreO = new int[mBoxCount];
	int k = 0;
	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			if (mStage[i * mWidth + j] == 'O')
			{
				mStage[i * mWidth + j] = ' ';
				mO[k] = j + mWidth * i;
				mPreO[k] = j + mWidth * i;
				++k;
			}
		}
	}
}

Stage::~Stage()
{
	delete[] mO;
	delete[] mPreO;
	delete[] mQ;
	delete[] mStage;
	delete[] mFlagStage;
	delete mImage;
}

int Stage::Input()
{
	if (mbO == true || mbP == true)
	{
		return 0;
	}

	int move;
	auto keyboard = Input::Manager::instance().keyboard();
	bool inputW = keyboard.isTriggered('w') || keyboard.isTriggered('W');
	bool inputS = keyboard.isTriggered('s') || keyboard.isTriggered('S');
	bool inputA = keyboard.isTriggered('a') || keyboard.isTriggered('A');
	bool inputD = keyboard.isTriggered('d') || keyboard.isTriggered('D');

	if (inputA)
	{
		move = -1;
	}
	else if (inputD)
	{
		move = +1;
	}
	else if (inputW)
	{
		move = -mWidth;
	}
	else if (inputS)
	{
		move = +mWidth;
	}
	else
	{
		move = 0;
	}

	// ���� ��ġ ���
	mPreP = mP;
	for (int i = 0; i < mBoxCount; i++)
	{
		mPreO[i] = mO[i];
	}

	mP += move;
	return move;
}

void Stage::Update(int move)
{
	if (mbO == true || mbP == true)
	{
		return;
	}

	for (int i = 0; i < mBoxCount; i++)
	{
		if (mQ[i] == mP) // Q�� ���� �ѹ��� �ִ��� Ȯ��
		{
			int indexOfq = mP + move;	// p�� �� ��ŭ

			// Q�� Q�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mBoxCount; j++)
			{
				if (mQ[j] == indexOfq)
				{
					mP -= move;
					return;
				}
			}

			// Q�� O�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mBoxCount; j++)
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
					for (int k = 0; k < mBoxCount; k++)
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
			for (int j = 0; j < mBoxCount; j++)
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

	for (int i = 0; i < mBoxCount; i++)
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
					for (int k = 0; k < mBoxCount; k++)
					{
						if (mO[k] == indexOfo)
						{
							mP -= move;
							return;
						}
					}
					mO[i] += move;

					for (int k = 0; k < mBoxCount; k++)
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
			for (int j = 0; j < mBoxCount; j++)
			{
				if (mQ[j] == indexOfo)
				{
					mP -= move;
					return;
				}
			}

			// O�� O�� �浹�ߴ��� Ȯ��
			for (int j = 0; j < mBoxCount; j++)
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

void Stage::Draw()
{
	const unsigned int TILE_SIZE = 32;
	double movedPixel = 1.;

	// ����, ��, Ÿ�� ���� �׸���
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			char data = mStage[y * mWidth + x];
			EFlag image;

			switch (data)
			{
			case ' ':
			case '.':
			{
				image = EFlag::Space;
				break;
			}
			case 'w':
			{
				image = EFlag::Wall;
				break;
			}
			case 'x':
			{
				image = EFlag::Target;
				break;
			}
			default:
			{
				image = EFlag::Space;
				break;
			}
			}

			mFlagStage[y * mWidth + x] = image;
		}
	}

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			int tX = x * TILE_SIZE;
			int tY = y * TILE_SIZE;
			EFlag drawStage = mFlagStage[y * mWidth + x];

			switch (drawStage)
			{
			case EFlag::Wall:
			{
				mImage->Draw(tX, tY, TILE_SIZE * 1, 0, TILE_SIZE * 2, TILE_SIZE);
				break;
			}
			case EFlag::Target:
			{
				mImage->Draw(tX, tY, TILE_SIZE * 3, 0, TILE_SIZE * 4, TILE_SIZE);
				break;
			}
			default:
			{
				mImage->Draw(tX, tY, TILE_SIZE * 4, 0, TILE_SIZE * 5, TILE_SIZE);
				break;
			}
			}
		}
	}

	// �÷��̾�, ���� ���߿� �׸���
	for (int i = 0; i < mBoxCount; i++)
	{
		if (mO[i] >= 0 && mO[i] < mWidth * mHeight)
		{
			int oX = 0, oY = 0;
			for (int y = 0; y < mHeight; y++)
			{
				for (int x = 0; x < mWidth; x++)
				{
					if (mPreO[i] == y * mWidth + x)
					{
						oX = x * TILE_SIZE;
						oY = y * TILE_SIZE;
					}
				}
			}

			int diff = mO[i] - mPreO[i];
			if (diff == -mWidth) // ��
			{
				mbO = true;
				mImage->Draw(oX, oY - mNumO, TILE_SIZE * 2, 0, TILE_SIZE * 3, TILE_SIZE);
				mNumO += movedPixel;
				if (mNumO > TILE_SIZE)
				{
					mbO = false;
					mNumO = 0;
				}
			}
			else if (diff == +mWidth) // �Ʒ�
			{
				mbO = true;
				mImage->Draw(oX, oY + mNumO, TILE_SIZE * 2, 0, TILE_SIZE * 3, TILE_SIZE);
				mNumO += movedPixel;
				if (mNumO > TILE_SIZE)
				{
					mbO = false;
					mNumO = 0;
				}
			}
			else if (diff == -1) // ����
			{
				mbO = true;
				mImage->Draw(oX - mNumO, oY, TILE_SIZE * 2, 0, TILE_SIZE * 3, TILE_SIZE);
				mNumO += movedPixel;
				if (mNumO > TILE_SIZE)
				{
					mbO = false;
					mNumO = 0;
				}
			}
			else if (diff == +1) // ������
			{
				mbO = true;
				mImage->Draw(oX + mNumO, oY, TILE_SIZE * 2, 0, TILE_SIZE * 3, TILE_SIZE);
				mNumO += movedPixel;
				if (mNumO > TILE_SIZE)
				{
					mbO = false;
					mNumO = 0;
				}
			}
			else
			{
				mImage->Draw(oX, oY, TILE_SIZE * 2, 0, TILE_SIZE * 3, TILE_SIZE);
			}
		}
	}

	if (mP >= 0 && mP < mWidth * mHeight)
	{
		int diff = mP - mPreP;
		int pX = 0, pY = 0;
		for (int y = 0; y < mHeight; y++)
		{
			for (int x = 0; x < mWidth; x++)
			{
				if (mPreP == y * mWidth + x)
				{
					pX = x * TILE_SIZE;
					pY = y * TILE_SIZE;
				}
			}
		}

		if (diff == -mWidth) // ��
		{
			mbP = true;
			mImage->Draw(pX, pY - mNumP, TILE_SIZE * 0, 0, TILE_SIZE * 1, TILE_SIZE);
			mNumP += movedPixel;
			if (mNumP > TILE_SIZE)
			{
				mbP = false;
				mNumP = 0;
			}
		}
		else if (diff == +mWidth) // �Ʒ�
		{
			mbP = true;
			mImage->Draw(pX, pY + mNumP, TILE_SIZE * 0, 0, TILE_SIZE * 1, TILE_SIZE);
			mNumP += movedPixel;
			if (mNumP > TILE_SIZE)
			{
				mbP = false;
				mNumP = 0;
			}
		}
		else if (diff == -1) // ����
		{
			mbP = true;
			mImage->Draw(pX - mNumP, pY, TILE_SIZE * 0, 0, TILE_SIZE * 1, TILE_SIZE);
			mNumP += movedPixel;
			if (mNumP > TILE_SIZE)
			{
				mbP = false;
				mNumP = 0;
			}
		}
		else if (diff == +1) // ������
		{
			mbP = true;
			mImage->Draw(pX + mNumP, pY, TILE_SIZE * 0, 0, TILE_SIZE * 1, TILE_SIZE);
			mNumP += movedPixel;
			if (mNumP > TILE_SIZE)
			{
				mbP = false;
				mNumP = 0;
			}
		}
		else
		{
			mImage->Draw(pX, pY, TILE_SIZE * 0, 0, TILE_SIZE * 1, TILE_SIZE);
		}
	}
}

bool Stage::IsClear() const
{
	int count = 0;
	for (int i = 0; i < mBoxCount; i++)
	{
		if (mQ[i] > 0)
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