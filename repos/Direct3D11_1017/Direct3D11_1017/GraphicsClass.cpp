#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
	: m_Direct3D(nullptr)
{
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
	: m_Direct3D(other.m_Direct3D)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	m_Direct3D = new D3DClass();
	if (!m_Direct3D)
	{
		return false;
	}

	bool result = m_Direct3D->Initialize(screenWidth, screenHeight,
		VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}
}

bool GraphicsClass::Frame()
{
	bool result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	m_Direct3D->BeginScene(1.0f, 1.0f, 0.0f, 1.0f);

	m_Direct3D->EndScene();

	return true;
}