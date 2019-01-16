#include "Skybox.h"
#include <minwinbase.h>

const LPCSTR shader_filename = "skybox_shader.hlsl"; // default shader filename

Skybox::~Skybox()
{

}

Skybox::Skybox()
{
}

// create skybox object with given texture
Skybox::Skybox(const LPCSTR texture_name) : GameObject(texture_name)
{
	UINT numverts; // vertex numbers
	POS_TEX_VERTEX* skybox_desc = Geometry::create_skybox(&numverts); // create skybox vertex data
	model_ = new Model(); // create new model without object file	
	model_->set_shader_file(shader_filename); // set models shader file
	model_->load_texture(texture_name); // load texture
	model_->load_geo_model(skybox_desc, numverts, sizeof(POS_TEX_VERTEX)); // load created geometry into the model
	init_depth_stencil_states(); // set up depth stencil state
	init_rasterizer(); // set up rasterizer

}

void Skybox::draw(const XMMATRIX view_projection, const D3D11_PRIMITIVE_TOPOLOGY mode) const
{
	//immediateContext->RSSetState(rasterizerSky);
	immediate_context_->OMSetDepthStencilState(depth_write_sky_, 0);
	model_->draw(transform_->get_world()* view_projection, mode);
	immediate_context_->OMSetDepthStencilState(depth_write_solid_, 0);
	//immediateContext->RSSetState(rasterizerSolid);
}

// using the Rasterizer results in weird outcome i.e. the sphere rotating when i move the camera
HRESULT Skybox::init_rasterizer()
{
	// rasterizer description
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // free memory
	
	desc.FillMode = D3D11_FILL_SOLID; // set solid state
	desc.CullMode = D3D11_CULL_BACK; // reverse culling to see inner faces
	
	auto hr = device_->CreateRasterizerState(&desc, &rasterizer_sky_); // create rasterizer state with device and store it in rasterizer_sky_

	desc.FillMode = D3D11_FILL_SOLID; // set solid state
	desc.CullMode = D3D11_CULL_FRONT; // change culling mode back to normal
	hr = device_->CreateRasterizerState(&desc, &rasterizer_solid_); // create rasterizer state with device and store it in rasterizer_solid_

	return hr;
}

// initialize depth stencil state
HRESULT Skybox::init_depth_stencil_states()
{
	// depth stencil description
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // free memory
	desc.DepthEnable = true; // enable depth writing
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // set write mask to maximum depth
	desc.DepthFunc = D3D11_COMPARISON_LESS; // set depth function
	auto hr = device_->CreateDepthStencilState(&desc, &depth_write_solid_); // create depth stencil 
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // set write mask to 0 depth
	hr = device_->CreateDepthStencilState(&desc, &depth_write_sky_); // create depth stencil state and store it into depth_write_sky_
	return hr;
}

void Skybox::cleanup() const
{
	if (depth_write_solid_) depth_write_solid_->Release();
	if (depth_write_sky_) depth_write_sky_->Release();
}

