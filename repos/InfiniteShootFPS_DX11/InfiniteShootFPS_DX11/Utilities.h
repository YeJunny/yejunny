#pragma once

#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Utilities
{
public:
	static json ParseJsonFile(const char* filePath);
};

