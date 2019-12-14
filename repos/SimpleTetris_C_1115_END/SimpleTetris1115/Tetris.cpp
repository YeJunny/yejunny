#include <stdio.h>
#include "Global.h"
#include "Variable.h"

void Initialize();
bool CheckBlockCollision(int x, int y);
bool CheckWallCollision(int leftRight);
void EraseBlockFullLine();
bool IsGameOver();
void RemoveBlock();
void PressKey();
void UpdateBlock();
int GetBlockElement(int x, int y);
void Draw();

int gBaseBlock;
int gNumBlock;
int gMapX, gMapY;
bool gHasCreate = true;

int main()
{
	// Setting
	ShowCursorView(0);
	srand(time(NULL));
	
	while (true)
	{
		if (gHasCreate)
		{	
			Initialize();
		}

		if (IsGameOver())
		{
			break;
		}

		RemoveBlock();

		PressKey();

		if (CheckBlockCollision(0, 1))
		{
			gHasCreate = true;
			UpdateBlock();
		}
		else
		{
			gMapY++;
			UpdateBlock();

			// 내린 후에 충돌했는지 확인
			if (CheckBlockCollision(0, 1))
			{
				gHasCreate = true;
				UpdateBlock();
			}
		}

		EraseBlockFullLine();

		Draw();

		Sleep(100);
	}

	return 0;
}

void Initialize()
{
	gBaseBlock = rand() % 7;
	gNumBlock = rand() % 4;

	int mX = 4, mY = 0;
	gMapX = mX, gMapY = mY;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			int element = GetBlockElement(x, y);
			if (element)
			{
				gMapArray[mY + y][mX + x] = element;
			}
		}
	}

	gHasCreate = false;
}

void EraseBlockFullLine()
{
	int count = 0;
	for (int y = 0; y < MAP_HEIGHT - 1; y++)
	{
		for (int x = 1; x < MAP_WIDTH - 1; x++)
		{
			if (gMapArray[y][x] == 7)
			{
				count++;
			}

			if (count % (MAP_WIDTH - 2) == 0 && count)
			{
				for (int dy = y; dy > 0; --dy)
				{
					for (int dx = 1; dx < MAP_WIDTH - 1; ++dx)
					{
						gMapArray[dy][dx] = gMapArray[dy - 1][dx];
					}
				}

				for (int dx = 1; dx < MAP_WIDTH - 1; ++dx)
				{
					gMapArray[0][dx] = 0;
				}
			}
		}
		count = 0;
	}
}

bool CheckBlockCollision(int x, int y)
{
	int mX = gMapX + x, mY = gMapY + y;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			int element = GetBlockElement(x, y);
			if (element &&
				(gMapArray[mY + y][mX + x] == 8 ||
				gMapArray[mY + y][mX + x] == 7))
			{
				return true;
			}
		}
	}

	return false;
}

void RemoveBlock()
{
	int mX = gMapX, mY = gMapY;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (gMapArray[mY + y][mX + x] == 1)
			{
				gMapArray[mY + y][mX + x] = 0;
			}
		}
	}
}

bool CheckWallCollision(int leftRight)
{
	int mX = gMapX + leftRight, mY = gMapY;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			int element = GetBlockElement(x, y);
			if (element &&
				gMapArray[mY + y][mX + x] == 9)
			{
				return true;
			}
		}
	}

	return false;
}

void PressKey()
{
	int keyLeft = GetAsyncKeyState(VK_LEFT);
	int keyRight = GetAsyncKeyState(VK_RIGHT);
	int keyUp = GetAsyncKeyState(VK_UP);
	int keySpace = GetAsyncKeyState(VK_SPACE);

	if (keySpace)
	{
		int y = 1;
		while (true)
		{
			if (!CheckBlockCollision(0, y))
			{
				y++;
			}
			else
			{
				gMapY += y - 1;
				break;
			}
		}
	}

	if (keyLeft)
	{
		if (!CheckWallCollision(-1) && !CheckBlockCollision(-1, 0))
		{
			gMapX--;
		}
	}

	if (keyRight)
	{
		if (!CheckWallCollision(1) && !CheckBlockCollision(1, 0))
		{
			gMapX++;
		}
	}

	if (keyUp)
	{
		gNumBlock++;
		if (gNumBlock >= 4)
		{
			gNumBlock = 0;
		}

		if (CheckWallCollision(0))
		{
			gNumBlock--;
			if (gNumBlock < 0)
			{
				gNumBlock = 3;
			}
		}
	}
}

void UpdateBlock()
{
	int mX = gMapX, mY = gMapY;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			int element = GetBlockElement(x, y);
			if (element)
			{
				if (!gHasCreate)
				{
					gMapArray[mY + y][mX + x] = 1;
				}
				else
				{
					gMapArray[mY + y][mX + x] = 7;
				}
			}
		}
	}
}

int GetBlockElement(int x, int y)
{
	int element;

	if (gBaseBlock == 0)
	{
		element = I_BLOCK[gNumBlock][y][x];
	}
	else if (gBaseBlock == 1)
	{
		element = T_BLOCK[gNumBlock][y][x];
	}
	else if (gBaseBlock == 2)
	{
		element = L_BLOCK[gNumBlock][y][x];
	}
	else if (gBaseBlock == 3)
	{
		element = J_BLOCK[gNumBlock][y][x];
	}
	else if (gBaseBlock == 4)
	{
		element = O_BLOCK[gNumBlock][y][x];
	}
	else if (gBaseBlock == 5)
	{
		element = S_BLOCK[gNumBlock][y][x];
	}
	else
	{
		element = Z_BLOCK[gNumBlock][y][x];
	}

	return element;
}

void Draw()
{
	GoToXY(10, 3);

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			int mapElement = gMapArray[y][x];
			if (mapElement == 9 || mapElement == 8)
			{
				ChangeTextColor(2, 1);
				printf("▣");
				ChangeTextColor(6, 1);
			}
			else if (mapElement == 1)
			{
				ChangeTextColor(13 , 1);
				printf("■");
				ChangeTextColor(6, 1);
			}
			else if (mapElement == 7)
			{
				ChangeTextColor(11, 1);
				printf("■");
				ChangeTextColor(6, 1);
			}
			else if (mapElement == 0)
			{
				printf("  ");
			}

			if (x == MAP_WIDTH - 1)
			{
				printf("\n");
				GoToXY(10, 3 + y);
			}
		}
	}
}

bool IsGameOver()
{
	if (CheckBlockCollision(0, 1))
	{
		Draw();
		GoToXY(20, 30);
		printf("GAME OVER");
		return true;
	}

	return false;
}
