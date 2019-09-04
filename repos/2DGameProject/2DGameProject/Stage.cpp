#define _CRT_SECURE_NO_WARNINGS

#include "Stage.h"

Stage::Stage(const char* stage, const char* image)
{
	// 맵 데이터 읽어오기
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

void Stage::Draw() const
{
	unsigned int* vram = Framework::instance().videoMemory();
	unsigned int windowWidth = Framework::instance().width();
	unsigned int windowHeight = Framework::instance().height();
	const unsigned int tileSize = 32;

	// 공백, 벽, 타켓 먼저 그리기
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
	
	for (int y = 0; y < mHeight * tileSize; y += tileSize)
	{
		for (int x = 0; x < mWidth * tileSize; x += tileSize)
		{
			EDraw drawStage = mDrawStage[(y / tileSize) * mWidth + (x / tileSize)];
			switch (drawStage)
			{
			case EDraw::Wall:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = tileSize; dx < tileSize * 2; dx++)
					{
						unsigned int imageData = mImage->GetData(dy * mImage->GetWidth() + dx);
						if (imageData & 0x80000000)
						{
							unsigned int* vramPtr = &vram[(y + dy) * windowWidth + x + (dx - tileSize)];
							*vramPtr = imageData;
						}
					}
				}
				break;
			}
			case EDraw::Target:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = tileSize * 3; dx < tileSize * 4; dx++)
					{
						unsigned int imageData = mImage->GetData(dy * mImage->GetWidth() + dx);
						if (imageData & 0x80000000)
						{
							unsigned int* vramPtr = &vram[(y + dy) * windowWidth + x + (dx - tileSize * 3)];
							*vramPtr = imageData;
						}
					}
				}
				break;
			}
			default:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = tileSize * 4; dx < tileSize * 5; dx++)
					{
						unsigned int imageData = mImage->GetData(dy * mImage->GetWidth() + dx);
						if (imageData & 0x80000000)
						{
							unsigned int* vramPtr = &vram[(y + dy) * windowWidth + x + (dx - tileSize * 4)];
							*vramPtr = imageData;
						}
					}
				}
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
			*(mDrawStage + mO[i]) = EDraw::Box; //'O';
		}
	}

	if (mP >= 0 && mP < mWidth * mHeight)
	{
		*(mDrawStage + mP) = EDraw::Player; // 'p';
	}
	
	for (int y = 0; y < mHeight * tileSize; y += tileSize)
	{
		for (int x = 0; x < mWidth * tileSize; x += tileSize)
		{
			EDraw drawStage = mDrawStage[(y / tileSize) * mWidth + (x / tileSize)];
			switch (drawStage)
			{
			case EDraw::Player:
			{
				for (int dy = 0; dy < tileSize; dy++)
				{
					for (int dx = 0; dx < tileSize; dx++)
					{
						unsigned int imageData = mImage->GetData(dy * mImage->GetWidth() + dx);
						if (imageData & 0x80000000)
						{
							unsigned int* vramPtr = &vram[(y + dy) * windowWidth + x + dx];
							*vramPtr = imageData;
						}
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
						unsigned int imageData = mImage->GetData(dy * mImage->GetWidth() + dx);
						if (imageData & 0x80000000)
						{
							unsigned int* vramPtr = &vram[(y + dy) * windowWidth + x + (dx - tileSize * 2)];
							*vramPtr = imageData;
						}
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