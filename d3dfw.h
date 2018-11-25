#pragma once
#include "Input.h"
class Input;
class d3dfw
{
public:
	HINSTANCE hInst;
	HWND hWnd;

	static d3dfw* getInstance();
	HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow, WNDPROC wndProc);
	HRESULT InitialiseD3D();
	HRESULT InitialiseInput();
	void ClearRTV();
	void Cleanup();
	Input* input;

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain = NULL;
private:
	d3dfw();
	~d3dfw();
	static d3dfw* instance;
	char g_Title[100] = "Swing to Win(g)";


	ID3D11Buffer* vertexBuffer;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3DBlob *VS, *PS;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* constantBuffer;


	D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;


	ID3D11RenderTargetView* renderTargetView = NULL;
	ID3D11Buffer* constantBuffer0;

	ID3D11DepthStencilView* zBuffer;


};

