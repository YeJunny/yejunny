#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(std::string message)
{
	std::string errorMessage = 
		"************************************\nError : "
		+ message 
		+ "\n************************************\n";
	MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR);
	assert(nullptr);
	OutputDebugStringA(errorMessage.c_str());
}

void ErrorLogger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring errorMessage = 
		L"************************************\nError : " 
		+ StringConverter::StringToWide(message) 
		+ L"\n" + error.ErrorMessage() 
		+ L"\n************************************\n";;
	MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
	assert(nullptr);
	OutputDebugStringW(errorMessage.c_str());
}

void ErrorLogger::AssertInitializationInternal(HRESULT hr, std::string message)
{
	if (FAILED(hr))
	{
		Log(hr, message);
		exit(EXIT_FAILURE);
	}
}
