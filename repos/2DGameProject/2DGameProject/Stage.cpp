#define _CRT_SECURE_NO_WARNINGS

#include "File.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include "Stage.h"

using namespace GameLib;
using namespace std;

Stage::Stage(const char* stage, const char* image)
	: mCountOfO(0.)
	, mCountOfP(0.)
	, mDrawP(false)
	, mDrawO(false)
{
	// 맵 데이터 읽어오기
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
	mDrawStage = new EDraw[length];
	memcpy(mStage, temp, length);

	delete[] temp;
	delete file;

	// 이미지 데이터 읽어오기
	mImage = new Image(image);


	// 멤버변수 초기화
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

	mO = new int[mNumOfBox];
	mPreO = new int[mNumOfBox];
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
	delete[] mDrawStage;
	delete mImage;
}

int Stage::Input()
{
	if (mDrawO == true || mDrawP == true)
	{
		return 0;
	}

	int move;
	auto f = Framework::instance();
	bool inputW = f.isKeyOn('w') || f.isKeyOn('W');
	bool inputS = f.isKeyOn('s') || f.isKeyOn('S');
	bool inputA = f.isKeyOn('a') || f.isKeyOn('A');
	bool inputD = f.isKeyOn('d') || f.isKeyOn('D');

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

	// 이전 위치 기억
	mPreP = mP;
	for (int i = 0; i < mNumOfBox; i++)
	{
		mPreO[i] = mO[i];
	}

	mP += move;
	return move;
}

void Stage::Update(int move)
{
	if (mDrawO == true || mDrawP == true)
	{
		return;
	}

	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mQ[i] == mP) // Q에 같은 넘버가 있는지 확인
		{
			int indexOfq = mP + move;	// p가 민 만큼

			// Q가 Q와 충돌했는지 확인
			for (int j = 0; j < mNumOfBox; j++)
			{
				if (mQ[j] == indexOfq)
				{
					mP -= move;
					return;
				}
			}

			// Q가 O와 충돌했는지 확인
			for (int j = 0; j < mNumOfBox; j++)
			{
				if (mO[j] == indexOfq)
				{
					mP -= move;
					return;
				}
			}

			// Q 관점에서,
			for (int j = 0; j < mWidth * mHeight; j++)
			{
				if (*(mStage + j) == 'w' && j == indexOfq)	// Q가 벽에 충돌했는지 확인
				{
					indexOfq -= move;
					mP -= move;
					return;
				}

				if (*(mStage + j) == 'x' && j == indexOfq)	// Q가 x와 충돌했는지 확인
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

			// 빈칸인 경우
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
		if (mO[i] == mP)	// O에 같은 넘버가 있는지 확인
		{
			int indexOfo = mP + move;	// p가 민 만큼

			// O 관점에서,
			for (int j = 0; j < mWidth * mHeight; j++)
			{
				if (*(mStage + j) == 'w' && j == indexOfo)	// O가 벽에 충돌했는지 확인
				{
					indexOfo -= move;
					mP -= move;
					return;
				}

				if (*(mStage + j) == 'x' && j == indexOfo)	// O가 x와 충돌했는지 확인
				{
					for (int k = 0; k < mNumOfBox; k++)
					{
						if (mO[k] == indexOfo)
						{
							mP -= move;
							return;
						}
					}
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

			// O가 Q와 충돌했는지 확인
			for (int j = 0; j < mNumOfBox; j++)
			{
				if (mQ[j] == indexOfo)
				{
					mP -= move;
					return;
				}
			}

			// O가 O와 충돌했는지 확인
			for (int j = 0; j < mNumOfBox; j++)
			{
				if (mO[j] == indexOfo)
				{
					mP -= move;
					return;
				}
			}

			// 빈칸인 경우
			mO[i] += move;
		}
	}

	for (int i = 0; i < mWidth * mHeight; i++) // 벽에 닿았는지 확인
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

	// 공백, 벽, 타켓 먼저 그리기
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			char data = mStage[y * mWidth + x];
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
				image =EDraw::Wall;
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

			mDrawStage[y * mWidth + x] = image;
		}
	}

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			int tX = x * TILE_SIZE;
			int tY = y * TILE_SIZE;
			EDraw drawStage = mDrawStage[y * mWidth + x];

			switch (drawStage)
			{
			case EDraw::Wall:
			{
				mImage->Draw(tX, tY, TILE_SIZE * 1, 0, TILE_SIZE * 2, TILE_SIZE);
				break;
			}
			case EDraw::Target:
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

	// 플레이어, 상자 나중에 그리기
	for (int i = 0; i < mNumOfBox; i++)
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
			if (diff == -mWidth) // 위
			{
				mDrawO = true;
				mImage->Draw(oX, oY - mCountOfO, TILE_SIZE * 2, 0, TILE_SIZE * 3, TILE_SIZE);
				mCountOfO += movedPixel;
				if (mCountOfO > TILE_SIZE)
				{
					mDrawO = false;
					mCountOfO = 0;
				}
			}
			else if (diff == +mWidth) // 아래
			{
				mDrawO = true;
				mImage->Draw(oX, oY + mCountOfO, TILE_SIZE * 2, 0, TILE_SIZE * 3, TILE_SIZE);
				mCountOfO += movedPixel;
				if (mCountOfO > TILE_SIZE)
				{
					mDrawO = false;
					mCountOfO = 0;
				}
			}
			else if (diff == -1) // 왼쪽
			{
				mDrawO = true;
				mImage->Draw(oX - mCountOfO, oY, TILE_SIZE * 2, 0, TILE_SIZE * 3, TILE_SIZE);
				mCountOfO += movedPixel;
				if (mCountOfO > TILE_SIZE)
				{
					mDrawO = false;
					mCountOfO = 0;
				}
			}
			else if (diff == +1) // 오른쪽
			{
				mDrawO = true;
				mImage->Draw(oX + mCountOfO, oY, TILE_SIZE * 2, 0, TILE_SIZE * 3, TILE_SIZE);
				mCountOfO += movedPixel;
				if (mCountOfO > TILE_SIZE)
				{
					mDrawO = false;
					mCountOfO = 0;
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

		if (diff == -mWidth) // 위
		{
			mDrawP = true;
			mImage->Draw(pX, pY - mCountOfP, TILE_SIZE * 0, 0, TILE_SIZE * 1, TILE_SIZE);
			mCountOfP += movedPixel;
			if (mCountOfP > TILE_SIZE)
			{
				mDrawP = false;
				mCountOfP = 0;
			}
		}
		else if (diff == +mWidth) // 아래
		{
			mDrawP = true;
			mImage->Draw(pX, pY + mCountOfP, TILE_SIZE * 0, 0, TILE_SIZE * 1, TILE_SIZE);
			mCountOfP += movedPixel;
			if (mCountOfP > TILE_SIZE)
			{
				mDrawP = false;
				mCountOfP = 0;
			}
		}
		else if (diff == -1) // 왼쪽
		{
			mDrawP = true;
			mImage->Draw(pX - mCountOfP, pY, TILE_SIZE * 0, 0, TILE_SIZE * 1, TILE_SIZE);
			mCountOfP += movedPixel;
			if (mCountOfP > TILE_SIZE)
			{
				mDrawP = false;
				mCountOfP = 0;
			}
		}
		else if (diff == +1) // 오른쪽
		{
			mDrawP = true;
			mImage->Draw(pX + mCountOfP, pY, TILE_SIZE * 0, 0, TILE_SIZE * 1, TILE_SIZE);
			mCountOfP += movedPixel;
			if (mCountOfP > TILE_SIZE)
			{
				mDrawP = false;
				mCountOfP = 0;
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