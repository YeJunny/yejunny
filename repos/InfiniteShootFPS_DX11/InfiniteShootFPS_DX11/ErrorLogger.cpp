#include "ErrorLogger.h"
#include <comdef.h>
#include <cassert>

void ErrorLogger::Log(std::string message)
{
	std::string errorMessage = 
		"************************************\nError : "
		+ message 
		+ "\n************************************\n";
	OutputDebugStringA(errorMessage.c_str());
	MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR);
	assert(nullptr);
}

void ErrorLogger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring errorMessage = 
		L"************************************\nError : " 
		+ StringConverter::StringToWide(message) 
		+ L"\n" + error.ErrorMessage() 
		+ L"\n************************************\n";;
	OutputDebugStringW(errorMessage.c_str());
	MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
	assert(nullptr);
}

void ErrorLogger::AssertInitializationInternal(HRESULT hr, std::string message)
{
	if (FAILED(hr))
	{
		Log(hr, message);
		exit(EXIT_FAILURE);
	}
}

void ErrorLogger::AssertIsSuccessInternal(bool bIsSuccess, std::string message)
{
	if (!bIsSuccess)
	{
		Log(message);
		exit(EXIT_FAILURE);
	}
}
