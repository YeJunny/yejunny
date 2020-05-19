#pragma once

#include "../Settings/Include/Framework.h"
using namespace DirectX;
using namespace Microsoft::WRL;

#define BULLET_COUNT (20)
#define TEXTURE_LEN (512)
#define Assert(x) assert(x)

class Input;
class Time;

class Global
{
public:
	static void CreateGlobal();
	static void DestroyGlobal();

	static HRESULT CompileShaderFromFile(
		const WCHAR* szFileName, const LPCSTR szEntryPoint,
		const LPCSTR szShaderModel, ID3DBlob** ppBlobout);

	// Getter
	static ComPtr<ID3D11Device> GetDevice();
	static ComPtr<ID3D11DeviceContext> GetContext();
	static ComPtr<IDXGISwapChain> GetSwapChain();
	static ComPtr<ID3D11RenderTargetView> GetRenderTargetView();
	static ComPtr<ID3D11DepthStencilView> GetDepthStencilView();
	static HWND& GetHwnd();
	static HINSTANCE& GetHinst();
	static FLOAT& GetWidth();
	static FLOAT& GetHeight();
	static XMMATRIX& GetViewMat();
	static XMMATRIX& GetProjectionMat();
	static DWORD& GetFrame();
	static std::unique_ptr<Input>& GetInput();
	static std::unique_ptr<Time>& GetTime();
	static XMFLOAT3& GetPlayerPos();
	static XMFLOAT3& GetPlayerRot();

	// Setter
	static void SetHwnd(HWND hWnd);
	static void SetHinst(HINSTANCE hInst);
	static void SetViewMat(XMMATRIX viewMat);
	static void SetFrame(DWORD frame);
	static void SetPlayerPos(XMFLOAT3 playerPos);
	static void SetPlayerRot(XMFLOAT3 playerRot);

private:
	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> Context;
	static ComPtr<IDXGISwapChain> SwapChain;
	static ComPtr<ID3D11Texture2D> DepthStencil;
	static ComPtr<ID3D11RenderTargetView> RenderTargetView;
	static ComPtr<ID3D11DepthStencilView> DepthStencilView;

	static HWND Hwnd;
	static HINSTANCE Hinst;

	static FLOAT Width;
	static FLOAT Height;

	static XMMATRIX ViewMat;
	static XMMATRIX ProjectionMat;

	static DWORD Frame;

	static std::unique_ptr<Input> InputManager;
	static std::unique_ptr<Time> TimeManager;

	static XMFLOAT3 PlayerPos;
	static XMFLOAT3 PlayerRot;

};
