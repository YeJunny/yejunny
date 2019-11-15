#define _CRT_SECURE_NO_WARNINGS
#define STR_LEN 256
#include <cstring>
#include <fstream>
#include "Stage.h"

using namespace std;


int main()
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
	int width = atoi(token);

	token = strtok(nullptr, "\n\r");
	int height = atoi(token);

	char* temp = new char[STR_LEN];
	token = strtok(nullptr, "\n\r");
	strcpy(temp, token);
	token = strtok(nullptr, "\n\r");

	while (token != nullptr)
	{
		strcat(temp, token);
		token = strtok(nullptr, "\n\r");
	}
	
	Stage stage(width, height, temp);

	delete[] str;
	delete[] temp;
	fin.close();

	stage.Draw();
	while (true)
	{
		int move = stage.Input();
		stage.Update(move);
		stage.Draw();

		bool bIsClear = stage.IsClear();
		if (bIsClear == true)
		{
			break;
		}
	}
}
