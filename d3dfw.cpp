#include "pch.h"
#include "d3dfw.h"
#include "Model.h"


d3dfw* d3dfw::instance = nullptr;

d3dfw::d3dfw()
{
}

d3dfw * d3dfw::getInstance()
{
	if (instance == nullptr)
		instance = new d3dfw();
	return instance;
}

d3dfw::~d3dfw()
{
}

HRESULT d3dfw::InitialiseWindow(HINSTANCE hInstance, int nCmdShow, WNDPROC wndProc)
{
	// window title
	char Name[100] = "Swing around\0";

	// Register class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Needed for non-D3D apps
	wcex.lpszClassName = Name;

	if (!RegisterClassEx(&wcex)) return E_FAIL;

	// Create window
	hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	hWnd = CreateWindow(Name, g_Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return E_FAIL;

	ShowWindow(hWnd, nCmdShow);

	return S_OK;
}

HRESULT d3dfw::InitialiseD3D() {
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hWnd, &rc);
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
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &swapChain,
			&device, &g_featureLevel, &immediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;



	ID3D11Texture2D* bufferTexture = NULL;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&bufferTexture);

	if (FAILED(hr)) return hr;

	hr = device->CreateRenderTargetView(bufferTexture, NULL, &renderTargetView);

	bufferTexture->Release();

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
	hr = device->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

	if (FAILED(hr)) return hr;

	// create z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	device->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &zBuffer);
	pZBufferTexture->Release();

	immediateContext->OMSetRenderTargets(1, &renderTargetView, zBuffer);

	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	immediateContext->RSSetViewports(1, &viewport);


	return S_OK;
}

HRESULT d3dfw::InitialiseInput()
{
	input = new Input();
	return input->InitialiseInput(hInst, hWnd);
}

void d3dfw::ClearRTV()
{
	float rgba_clear_colour[4] = { 0.0f,0.0f,0.0f,1.0f };
	immediateContext->ClearRenderTargetView(renderTargetView, rgba_clear_colour);
	immediateContext->ClearDepthStencilView(zBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void d3dfw::Cleanup()
{
	if (device) device->Release();
	if (immediateContext) immediateContext->Release();
	if (vShader) vShader->Release();
	if (pShader) pShader->Release();
	if (VS) VS->Release();
	if (PS) PS->Release();
	if (inputLayout) inputLayout->Release();
	if (constantBuffer) constantBuffer->Release();
	if (constantBuffer0) constantBuffer0->Release();
	if (vertexBuffer) vertexBuffer->Release();
	if (renderTargetView) renderTargetView->Release();
	if (zBuffer) zBuffer->Release();
	if (swapChain) swapChain->Release();
}
