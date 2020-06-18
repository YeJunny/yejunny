#include "ErrorLogger.h"
#include "Utilities.h"
#include <fstream>

json Utilities::ParseJsonFile(const char* filePath)
{
	char* msg;

	try
	{
		std::ifstream modelsInit(filePath);

		modelsInit.seekg(0, modelsInit.end);
		int fileSize = modelsInit.tellg();
		modelsInit.seekg(0, modelsInit.beg);

		msg = new char[fileSize];
		modelsInit.read(msg, fileSize);
		modelsInit.close();

		for (int size = fileSize - 1; size >= 0; --size)
		{
			if (msg[size] == '}')
			{
				msg[size + 1] = 0;
				break;
			}
		}
	}
	catch (std::ifstream::failure e)
	{
		ErrorLogger::Log("File Error : " + std::string(filePath));
		ErrorLogger::Log(e.what());
	}

	json parsingModel;

	try
	{
		parsingModel = json::parse(msg);
	}
	catch (json::parse_error& e)
	{
		ErrorLogger::Log(e.what());
	}
	delete[] msg;


    return parsingModel;
}
