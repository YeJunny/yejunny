#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System = nullptr;
	bool result = false;

	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	result = System->Initialize();
	if (result)
	{
		// 무한루프
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}
