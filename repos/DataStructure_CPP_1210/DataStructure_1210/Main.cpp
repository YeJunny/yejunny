#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

int main()
{
	std::ifstream modelsInit("ModelsInitialization.json");

	modelsInit.seekg(0, modelsInit.end);
	int fileSize = modelsInit.tellg();
	modelsInit.seekg(0, modelsInit.beg);

	char* msg = new char[fileSize];
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

	json parsingModel;

	try
	{
		parsingModel = json::parse(msg);
	}
	catch (json::parse_error& e) 
	{
		cout << "message: " << e.what() << '\n'
			<< "exception id: " << e.id << '\n'
			<< "byte position of error: " << e.byte << std::endl;
	}
	delete[] msg;


	/////////////

	auto objects = parsingModel.items();

	for (auto iter = objects.begin(); iter != objects.end(); ++iter)
	{
		string objName = (*iter).key();
		json objValue = (*iter).value();

		string modelPath = objValue["ModelPath"];
		string shaderPath = objValue["ShaderPath"];
		vector<string> textures = objValue["Textures"].get<vector<string>>();


		for (auto iter2 = textures.begin(); iter2 != textures.end(); ++iter2)
		{
			cout << (*iter2).c_str() << endl;
		}
	}


	return 0;
}
