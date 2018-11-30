#include "Skybox.h"
#include <minwinbase.h>

Skybox::~Skybox()
{

}

Skybox::Skybox()
{
}

Skybox::Skybox(LPCSTR texturename) : GameObject(name)
{
	POS_TEX_VERTEX* skybox_desc = Geometry::create_textured_skybox(20.0f);
	model = Model(dx_handle->device, dx_handle->immediateContext);
	model.LoadTexture(texturename);
	model.LoadGeoModel(skybox_desc, 36, sizeof(POS_TEX_VERTEX));
	InitDepthStencilStates();
	InitRasterizer();

}

Skybox::Skybox(LPCSTR name, Transform transform, Model model)
{

}

void Skybox::Draw(XMMATRIX view_projection, D3D11_PRIMITIVE_TOPOLOGY mode)
{
	immediateContext->OMSetDepthStencilState(depthWriteSky,0);
	model.Draw(transform.world* view_projection, mode);
	immediateContext->OMSetDepthStencilState(depthWriteSolid, 0);
}

// using the Rasterizer results in weird outcome i.e. the sphere rotating when i move the camera
HRESULT Skybox::InitRasterizer()
{
	HRESULT hr = NULL;
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	hr = device->CreateRasterizerState(&desc, &rasterizerSolid);

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_FRONT;
	hr = device->CreateRasterizerState(&desc, &rasterizerSky);

	return hr;
}

HRESULT Skybox::InitDepthStencilStates()
{
	HRESULT hr = NULL;
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = device->CreateDepthStencilState(&desc, &depthWriteSolid);
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = device->CreateDepthStencilState(&desc, &depthWriteSky);
	return hr;
}

void Skybox::Cleanup()
{
	if (depthWriteSolid) depthWriteSolid->Release();
	if (depthWriteSky) depthWriteSky->Release();
}