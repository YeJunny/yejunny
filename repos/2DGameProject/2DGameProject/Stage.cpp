#define _CRT_SECURE_NO_WARNINGS

#include "File.h"
#include "Image.h"
#include "Stage.h"

Stage::Stage(const char* stage, const char* image)
	: mCountOfO(0.)
	, mCountOfP(0.)
	, mDrawP(false)
	, mDrawO(false)
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

	// ���� ��ġ ���
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

void Stage::Draw(unsigned int diffTime)
{
	const unsigned int TILE_SIZE = 32;
	double movedPixel = diffTime / 16.;

	// ����, ��, Ÿ�� ���� �׸���
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
	
	for (int y = 0; y < mHeight * TILE_SIZE; y += TILE_SIZE)
	{
		for (int x = 0; x < mWidth * TILE_SIZE; x += TILE_SIZE)
		{
			EDraw drawStage = mDrawStage[(y / TILE_SIZE) * mWidth + (x / TILE_SIZE)];
			switch (drawStage)
			{
			case EDraw::Wall:
			{
				DrawTile(x, y, 1, TILE_SIZE);
				break;
			}
			case EDraw::Target:
			{
				DrawTile(x, y, 3, TILE_SIZE);
				break;
			}
			default:
			{
				DrawTile(x, y, 4, TILE_SIZE);
				break;
			}
			}
		}
	}

	// �÷��̾�, ���� ���߿� �׸���
	for (int i = 0; i < mNumOfBox; i++)
	{
		if (mO[i] >= 0 && mO[i] < mWidth * mHeight)
		{
			int oX, oY;
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
				mDrawO = true;
				DrawTile(oX, oY - mCountOfO, 2, TILE_SIZE);
				mCountOfO += movedPixel;
				if (mCountOfO > TILE_SIZE)
				{
					mDrawO = false;
					mCountOfO = 0;
				}
			}
			else if (diff == +mWidth) // �Ʒ�
			{
				mDrawO = true;
				DrawTile(oX, oY + mCountOfO, 2, TILE_SIZE);
				mCountOfO += movedPixel;
				if (mCountOfO > TILE_SIZE)
				{
					mDrawO = false;
					mCountOfO = 0;
				}
			}
			else if (diff == -1) // ����
			{
				mDrawO = true;
				DrawTile(oX - mCountOfO, oY, 2, TILE_SIZE);
				mCountOfO += movedPixel;
				if (mCountOfO > TILE_SIZE)
				{
					mDrawO = false;
					mCountOfO = 0;
				}
			}
			else if (diff == +1) // ������
			{
				mDrawO = true;
				DrawTile(oX + mCountOfO, oY, 2, TILE_SIZE);
				mCountOfO += movedPixel;
				if (mCountOfO > TILE_SIZE)
				{
					mDrawO = false;
					mCountOfO = 0;
				}
			}
			else
			{
				DrawTile(oX, oY, 2, TILE_SIZE);
			}
		}
	}

	if (mP >= 0 && mP < mWidth * mHeight)
	{
		int diff = mP - mPreP;
		int pX, pY;
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
			mDrawP = true;
			DrawTile(pX, pY - mCountOfP, 0, TILE_SIZE);
			mCountOfP += movedPixel;
			if (mCountOfP > TILE_SIZE)
			{
				mDrawP = false;
				mCountOfP = 0;
			}
		}
		else if (diff == +mWidth) // �Ʒ�
		{
			mDrawP = true;
			DrawTile(pX, pY + mCountOfP, 0, TILE_SIZE);
			mCountOfP += movedPixel;
			if (mCountOfP > TILE_SIZE)
			{
				mDrawP = false;
				mCountOfP = 0;
			}
		}
		else if (diff == -1) // ����
		{
			mDrawP = true;
			DrawTile(pX - mCountOfP, pY, 0, TILE_SIZE);
			mCountOfP += movedPixel;
			if (mCountOfP > TILE_SIZE)
			{
				mDrawP = false;
				mCountOfP = 0;
			}
		}
		else if (diff == +1) // ������
		{
			mDrawP = true;
			DrawTile(pX + mCountOfP, pY, 0, TILE_SIZE);
			mCountOfP += movedPixel;
			if (mCountOfP > TILE_SIZE)
			{
				mDrawP = false;
				mCountOfP = 0;
			}
		}
		else
		{
			DrawTile(pX, pY, 0, TILE_SIZE);
		}
	}
}

void Stage::DrawTile(int x, int y, int pos, int tileSize) const
{
	unsigned int* vram = Framework::instance().videoMemory();
	unsigned int windowWidth = Framework::instance().width();
	unsigned int windowHeight = Framework::instance().height();

	for (int dy = 0; dy < tileSize; dy++)
	{
		for (int dx = tileSize * pos; dx < tileSize * (pos + 1); dx++)
		{
			unsigned int src = mImage->GetData(dy * mImage->GetWidth() + dx);
			unsigned int* dst = &vram[(y + dy) * windowWidth + x + (dx - tileSize * pos)];

			unsigned int srcA = (src & 0xff000000) >> 24;
			unsigned int srcR = src & 0x00ff0000;
			unsigned int srcG = src & 0x0000ff00;
			unsigned int srcB = src & 0x000000ff;

			unsigned int dstR = *dst & 0x00ff0000;
			unsigned int dstG = *dst & 0x0000ff00;
			unsigned int dstB = *dst & 0x000000ff;

			unsigned int r = (srcR - dstR) * srcA / 255 + dstR;
			unsigned int g = (srcG - dstG) * srcA / 255 + dstG;
			unsigned int b = (srcB - dstB) * srcA / 255 + dstB;

			*dst = (r & 0x00ff0000) | (g & 0x0000ff00) | b;
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