#pragma once
#include "StringConverter.h"
#include <Windows.h>

#define AssertInitialization(hr, message) (ErrorLogger::AssertInitializationInternal(hr, message))

class ErrorLogger
{
public:
	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
	static void AssertInitializationInternal(HRESULT hr, std::string message);
};