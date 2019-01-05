#include "Skybox.h"
#include <minwinbase.h>

Skybox::~Skybox()
{

}

Skybox::Skybox()
{
}

Skybox::Skybox(const LPCSTR texture_name) : GameObject(texture_name)
{
	POS_TEX_VERTEX* skybox_desc = Geometry::create_textured_skybox(20.0f);
	model_ = Model(dx_handle_->device, dx_handle_->immediate_context);
	char filename[] = "skybox_shader.hlsl";
	model_.set_shader_file(filename);
	model_.LoadTexture(texture_name);
	model_.LoadGeoModel(skybox_desc, 36, sizeof(POS_TEX_VERTEX));
	init_depth_stencil_states();
	init_rasterizer();

}

Skybox::Skybox(const LPCSTR name, Transform transform, Model model): Skybox(name)
{
}

void Skybox::draw(const XMMATRIX view_projection, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	//immediateContext->RSSetState(rasterizerSky);
	immediate_context_->OMSetDepthStencilState(depth_write_sky_,0);
	model_.Draw(transform.world* view_projection, mode);
	immediate_context_->OMSetDepthStencilState(depth_write_solid_, 0);
	//immediateContext->RSSetState(rasterizerSolid);
}

// using the Rasterizer results in weird outcome i.e. the sphere rotating when i move the camera
HRESULT Skybox::init_rasterizer()
{
	HRESULT hr = NULL;
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	hr = device_->CreateRasterizerState(&desc, &rasterizer_sky_);

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_FRONT;
	hr = device_->CreateRasterizerState(&desc, &rasterizer_solid_);

	return hr;
}

HRESULT Skybox::init_depth_stencil_states()
{
	HRESULT hr = NULL;
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = device_->CreateDepthStencilState(&desc, &depth_write_solid_);
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = device_->CreateDepthStencilState(&desc, &depth_write_sky_);
	return hr;
}

void Skybox::cleanup() const
{
	if (depth_write_solid_) depth_write_solid_->Release();
	if (depth_write_sky_) depth_write_sky_->Release();
}

void Skybox::spawn(XMVECTOR position)
{
}

void Skybox::render()
{
}

void Skybox::start()
{
}

void Skybox::update(VGTime timer)
{
}

void Skybox::cleanup()
{
}
