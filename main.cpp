#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
//#include <iostream>
//#include <vector>
//#include <typeinfo>
//#include <io.h>


#include "VGTime.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Player.h"
#include "Camera.h"

//using namespace std;

struct POS_COL_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT4 Col;
};

struct CONSTANT_BUFFER0
{
	XMMATRIX WorldViewProjection; // 64 bytes
	float RedAmount; // 4 bytes
	float GreenAmount; // 4 bytes
	float BlueAmount; // 4 bytes
	float scale; // 4 bytes
	//XMFLOAT3 packing_bytes; // 12 bytes
}; // 80 bytes



// game objects
Camera* camera;
//std::vector<GameObject> gameObjects(2);


const float rot_speed = 200.0f;

CONSTANT_BUFFER0 cb0;

VGTime* timer;

HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;

D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* g_pD3DDevice = NULL;
ID3D11DeviceContext* g_pImmediateContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pBackBufferRTView = NULL;
ID3D11Buffer* g_pConstantBuffer0;

ID3D11Buffer* g_pVertexBuffer;
ID3D11VertexShader* g_pVertexShader;
ID3D11PixelShader* g_pPixelShader;
ID3D11InputLayout* g_pInputLayout;
ID3D11DepthStencilView* g_pZBuffer;



//g_Title is a replace for g_TutorialName
char g_Title[100] = "Swing to Win(g)";


HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitialiseD3D();
HRESULT InitialiseGraphics(void);
void ShutdownD3D();
void RenderFrame(void);
void AlterVertices(POS_COL_VERTEX* vert, WPARAM message);


// methods
void UpdateAI();
void UpdateInput();
void UpdateSound();
void UpdateGraphics();
void MoveCamera(WPARAM message);


POS_COL_VERTEX cube[] =
{
	// back face 
	{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, -1.0f, 1.0f),XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	// front face
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	// left face
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	// right face
	{XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	// bottom face
	{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	// top face
	{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, -1.0f),XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)}

};

POS_COL_VERTEX shape_1[] =
{
	{XMFLOAT3(0.2f,0.2f,0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(0.9f,-0.9f,0.0f), XMFLOAT4(0.2f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-0.9f,-0.9f,0.0f), XMFLOAT4(0.0f, 0.5f, 0.2f, 0.5f)}
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	camera = new Camera();

	timer = new VGTime();
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

	MSG msg = { 0 };

	if (FAILED(InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to initialise DirectX");
		return 0;
	}

	if (FAILED(InitialiseGraphics()))
	{
		DXTRACE_MSG("Failed to initialise graphics");
		return 0;
	}
	timer->start();

	while (msg.message != WM_QUIT)
	{
		timer->tick();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//UpdateAI();
			//UpdateInput();
			//UpdateSound();
			//UpdateGraphics();
			RenderFrame();
			//if (GetAsyncKeyState('A') & 0x0001)
			//{
			//	AlterVertices(vertices);
			//	//OutputDebugString("moved");
			//}
		}

	}

	timer->stop();
	return (int)msg.wParam;
}

void RenderFrame(void)
{
	double delta = timer->deltaTime();
	static bool glow;
	delta *= 10;

	if (cb0.RedAmount <= 0.01f || cb0.GreenAmount <= 0.01f || cb0.BlueAmount <= 0.01f) glow = true;
	else if (cb0.RedAmount >= 0.99f || cb0.GreenAmount >= 0.99f || cb0.BlueAmount >= 0.99f) glow = false;

	if (glow)
	{
		cb0.RedAmount += 0.1f * delta;
		cb0.GreenAmount += 0.1f * delta;
		cb0.BlueAmount += 0.1f * delta;
	}else
	{
		cb0.RedAmount -= 0.1f * delta;
		cb0.GreenAmount -= 0.1f * delta;
		cb0.BlueAmount -= 0.1f * delta;
	}


	
	//camera->look_at(XMVectorSet(0.0, 0.0, -4.0, 0.0));

	const XMMATRIX view_projection = camera->get_view_projection();
	//const XMMATRIX world = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(timer->totalTime() * 4, timer->totalTime() * 2, timer->totalTime() * 3));
	//cb0.WorldViewProjection = world * view_projection;
	cb0.WorldViewProjection = XMMatrixIdentity() * view_projection;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer0, 0, 0, &cb0, 0, 0);

	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer0);

	//float rgba_clear_colour[4] = { 0.1f,0.2f,0.6f,1.0f };
	float rgba_clear_colour[4] = { 0.0f,0.0f,0.0f,1.0f };
	g_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);
	g_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	UINT stride = sizeof(POS_COL_VERTEX);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pImmediateContext->Draw(36, 0);


	// RENDER HERE

	g_pSwapChain->Present(0, 0);
}


HRESULT InitialiseGraphics()
{
	HRESULT hr = S_OK;

	// create constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // can use UpdateSubresrource() to update
	constant_buffer_desc.ByteWidth = sizeof(cb0); // MUST be a multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = g_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &g_pConstantBuffer0);

	if (FAILED(hr))
		return hr;

	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //Dynamic -> used by CPU and GPU
	bufferDesc.ByteWidth = sizeof(cube); // Size of the buffer, 3 vertices
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow cpu access
	hr = g_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &g_pVertexBuffer); // create buffer

	if (FAILED(hr)) { 
		return hr; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;



	// Lock the buffer to allow writing
	g_pImmediateContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// copy the data
	memcpy(ms.pData, cube, sizeof(cube));

	// unlock the buffer
	g_pImmediateContext->Unmap(g_pVertexBuffer, NULL);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	// create shader objects
	hr = g_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &g_pVertexShader);
	hr = g_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &g_pPixelShader);

	// set the shader objects as active
	g_pImmediateContext->VSSetShader(g_pVertexShader, 0, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, 0, 0);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	hr = g_pD3DDevice->CreateInputLayout(iedesc, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &g_pInputLayout);

	g_pImmediateContext->IASetInputLayout(g_pInputLayout);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{

	// window title
	char Name[100] = "Hello World\0";

	// Register class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Needed for non-D3D apps
	wcex.lpszClassName = Name;

	if (!RegisterClassEx(&wcex)) return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(Name, g_Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		//AlterVertices(shape_1, wParam);
		MoveCamera(wParam);
		if (wParam == VK_ESCAPE)
			DestroyWindow(g_hWnd);

		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

HRESULT InitialiseD3D() {
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;


	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, // comment out this line if you need to test D3D 11.0 functionality on hardware that doesn't support it
		D3D_DRIVER_TYPE_WARP, // comment this out also to use reference device
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain,
			&g_pD3DDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;



	ID3D11Texture2D* pBackBufferTexture = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBufferTexture);

	if (FAILED(hr)) return hr;

	hr = g_pD3DDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &g_pBackBufferRTView);

	pBackBufferTexture->Release();

	if (FAILED(hr)) return hr;

	// create z buffer texture
	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));

	tex2dDesc.Width = width;
	tex2dDesc.Height = height;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = sd.SampleDesc.Count;
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D *pZBufferTexture;
	hr = g_pD3DDevice->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

	if (FAILED(hr)) return hr;

	// create z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	g_pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &g_pZBuffer);
	pZBufferTexture->Release();

	g_pImmediateContext->OMSetRenderTargets(1, &g_pBackBufferRTView, g_pZBuffer);

	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	g_pImmediateContext->RSSetViewports(1, &viewport);


	return S_OK;

}

void ShutdownD3D()
{
	
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pConstantBuffer0) g_pConstantBuffer0->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pD3DDevice) g_pD3DDevice->Release();
	if (g_pBackBufferRTView) g_pBackBufferRTView->Release();
}

void UpdateAI()
{

}

void UpdateInput()
{
	//for (GameObject var : gameObjects)
	//{
	//	//if (typeid(var) != typeid(Player)) continue;
	//	//std::cout << var.get_name();
	//}
	//std::cout << gameObjects[0].get_name();
}

void UpdateSound()
{

}

void UpdateGraphics()
{

}

void AlterVertices(POS_COL_VERTEX* vert, WPARAM message) {

	int size = sizeof(vert);
	//OutputDebugString("" + message);

	switch (message)
	{
	case VK_RIGHT:
		OutputDebugString("moved");
		for (int i = 0; i < size; i++)
		{
			vert[i].Pos.x += 0.01f;
			OutputDebugString("moved");
		}
		break;
	case VK_LEFT:
		for (int i = 0; i < size; i++)
		{
			vert[i].Pos.x -= 0.01f;
			OutputDebugString("moved");
		}
		break;
	case VK_UP:
		OutputDebugString("moved");
		for (int i = 0; i < size; i++)
		{
			vert[i].Pos.y += 0.01f;
			OutputDebugString("moved");
		}
		break;
	case VK_DOWN:
		for (int i = 0; i < size; i++)
		{
			vert[i].Pos.y -= 0.01f;
			OutputDebugString("moved");
		}
		break;
	}


	D3D11_MAPPED_SUBRESOURCE ms;

	// Lock the buffer to allow writing
	g_pImmediateContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// copy the data
	memcpy(ms.pData, cube, sizeof(cube));

	// unlock the buffer
	g_pImmediateContext->Unmap(g_pVertexBuffer, NULL);
}

float move_speed = 200.0f;

void MoveCamera(WPARAM message) {

	switch (message)
	{
	case VK_RIGHT:
		camera->rotate(0, timer->deltaTime() * rot_speed, 0);
		OutputDebugString("moved");
		break;
	case VK_LEFT:
		camera->rotate(0, -timer->deltaTime() * rot_speed, 0);		
		OutputDebugString("moved");
		break;
	case VK_UP:
		camera->move_forward(timer->deltaTime()*move_speed);		
		OutputDebugString("moved");
		break;
	case VK_DOWN:
		camera->move_forward(-timer->deltaTime()*move_speed);
		//camera->transform.translate(XMVECTOR(XMVectorSet(camera->transform.position.x, camera->transform.position.y, camera->transform.position.z - timer->deltaTime() * move_speed, 0)));
		OutputDebugString("moved");
		break;
	default: break;
	}
}
