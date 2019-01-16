#include "d3dfw.h"



D3Dfw* D3Dfw::instance_ = nullptr;

D3Dfw::~D3Dfw()
{

}

D3Dfw::D3Dfw(): h_inst(nullptr), h_wnd(nullptr), input(nullptr), device_(nullptr), immediate_context_(nullptr),
                z_buffer_(nullptr)
{
}

D3Dfw * D3Dfw::get_instance()
{
	if (instance_ == nullptr)
		instance_ = new D3Dfw();
	return instance_; // return singleton instance for any class needs access to it
}



HRESULT D3Dfw::initialise_window(const HINSTANCE instance, const int n_cmd_show, const WNDPROC wnd_proc)
{
	// window title
	char name[100] = "Swing around\0";

	// register window class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wnd_proc;
	wcex.hInstance = instance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); // Needed for non-D3D apps
	wcex.lpszClassName = name;

	if (!RegisterClassEx(&wcex)) return E_FAIL;

	// create window
	h_inst = instance;
	RECT rc = { 0, 0, screen_width_, screen_height_ }; // rectangle with corner positions
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE); // put the window rect to a specific position
	h_wnd = CreateWindow(name, g_title_, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, instance, NULL);
	if (!h_wnd)
		return E_FAIL;

	ShowWindow(h_wnd, n_cmd_show); // make window visible

	return S_OK;
}

HRESULT D3Dfw::initialise_dx() {
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(h_wnd, &rc);
	// width and height defined by the rectangle of the window
	const UINT width = rc.right - rc.left; // window width
	const UINT height = rc.bottom - rc.top; // window height



	// direct x driver types
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, 
		D3D_DRIVER_TYPE_WARP, 
		D3D_DRIVER_TYPE_REFERENCE,
	};
	const auto num_driver_types = ARRAYSIZE(driver_types); // number of driver types

	// supported feature levels
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	const auto num_feature_levels = ARRAYSIZE(feature_levels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = h_wnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	// loop through driver types and create device and swap chain connection. break when succeded
	const UINT create_device_flags = 0;
	for (auto& driver_type : driver_types)
	{
		driver_type_ = driver_type;
		hr = D3D11CreateDeviceAndSwapChain(nullptr, driver_type_, nullptr,
			create_device_flags, feature_levels, num_feature_levels,
			D3D11_SDK_VERSION, &sd, &swap_chain_,
			&device_, &feature_level_, &immediate_context_);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;

	
	// load swap chain buffer into texture resource
	ID3D11Texture2D* buffer_texture = nullptr;
	hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<LPVOID*>(&buffer_texture));

	if (FAILED(hr)) return hr;

	// create render target view using buffer texture
	hr = device_->CreateRenderTargetView(buffer_texture, nullptr, &render_target_view_);

	buffer_texture->Release(); // delete buffer texture

	if (FAILED(hr)) return hr;

	// z buffer texture description
	D3D11_TEXTURE2D_DESC tex_2d_desc;
	// free memory for the z buffer description
	ZeroMemory(&tex_2d_desc, sizeof(tex_2d_desc));

	tex_2d_desc.Width = width;
	tex_2d_desc.Height = height;
	tex_2d_desc.ArraySize = 1;
	tex_2d_desc.MipLevels = 1;
	tex_2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_2d_desc.SampleDesc.Count = sd.SampleDesc.Count;
	tex_2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_2d_desc.Usage = D3D11_USAGE_DEFAULT;

	// create z buffer texture
	ID3D11Texture2D *z_buffer_texture;
	hr = device_->CreateTexture2D(&tex_2d_desc, nullptr, &z_buffer_texture);

	if (FAILED(hr)) return hr;

	// create z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(dsv_desc));

	dsv_desc.Format = tex_2d_desc.Format; // set format for the depth stencil buffer
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // format of the depth stencil view dimension

	device_->CreateDepthStencilView(z_buffer_texture, &dsv_desc, &z_buffer_); // create depth stencil view
	z_buffer_texture->Release(); // delete z buffer texture

	immediate_context_->OMSetRenderTargets(1, &render_target_view_, z_buffer_); // set a single render target view

	// viewport structure
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0; // set top left x coordinate
	viewport.TopLeftY = 0; // set top left y coordinate
	viewport.Width = static_cast<FLOAT>(width); // define the width
	viewport.Height = static_cast<FLOAT>(height); // define the height
	viewport.MinDepth = 0.0f; // minimum depth of viewport
	viewport.MaxDepth = 1.0f; // maximum depth of viewport

	immediate_context_->RSSetViewports(1, &viewport); // create single viewport 


	return hr;
}

HRESULT D3Dfw::initialise_input()
{
	input = new Input(); // create input using program and window instance
	return input->initialise_input(h_inst, h_wnd);
}

void D3Dfw::clear_rtv() const
{
	float rgba_clear_colour[4] = { 0.0f,0.0f,0.0f,1.0f }; // set the clear color of the render target
	immediate_context_->ClearRenderTargetView(render_target_view_, rgba_clear_colour); // clear the render target with the color
	immediate_context_->ClearDepthStencilView(z_buffer_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); // clear z-buffer
}

ID3D11Device* D3Dfw::get_device() const
{
	return device_;
}

ID3D11DeviceContext* D3Dfw::get_immediate_context() const
{
	return immediate_context_;
}

IDXGISwapChain* D3Dfw::get_swap_chain() const
{
	return swap_chain_;
}

void D3Dfw::cleanup() const
{	
	if (device_) device_->Release();
	if (immediate_context_) immediate_context_->Release();
	if (render_target_view_) render_target_view_->Release();
	if (z_buffer_) z_buffer_->Release();
	if (swap_chain_) swap_chain_->Release();
}
