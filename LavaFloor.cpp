#include "pch.h"
#include "LavaFloor.h"


LavaFloor::LavaFloor()
{
}


LavaFloor::~LavaFloor()
{


}

LavaFloor::LavaFloor(const LPCSTR texture, const LPCSTR texture_normal, const LPCSTR texture_noise, const int tiles, const float scale) : Floor(tiles, scale)
{
	set_shader_file("lava_shader.hlsl");

	D3DX11CreateShaderResourceViewFromFile(device_, texture_normal, nullptr, nullptr, &normal_texture_, nullptr);
	D3DX11CreateShaderResourceViewFromFile(device_, texture_noise, nullptr, nullptr, &noise_texture_, nullptr);

	load_texture("assets/lava_selfmade_diffuse.png");
}



void LavaFloor::draw(const XMMATRIX view_projection, const bool use_default_cb, const D3D11_PRIMITIVE_TOPOLOGY mode) 
{
	immediate_context_->PSSetShaderResources(1, 1, &normal_texture_);
	immediate_context_->PSSetShaderResources(2, 1, &noise_texture_);
	Model::draw(view_projection, use_default_cb, mode);
}

