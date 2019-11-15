#pragma once

#include "D3DClass.h"

// GLOBALS
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();

	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

private:
	bool Render();

	D3DClass* m_Direct3D;
};