#include "pch.h"
#include "LavaFloor.h"

const LPCSTR shader_file = "lava_shader.hlsl";


LavaFloor::LavaFloor()
{
}


LavaFloor::~LavaFloor()
{


}

// initialize lava floor with texture, number of tiles, scale of the plane, texture normal, texture noise
LavaFloor::LavaFloor( const int tiles, const float scale, const LPCSTR texture, const LPCSTR texture_normal, const LPCSTR texture_noise) : Floor(tiles, scale)
{
	set_shader_file(shader_file); // set shader file

	D3DX11CreateShaderResourceViewFromFile(device_, texture_normal, nullptr, nullptr, &normal_texture_, nullptr); // load normal texture
	D3DX11CreateShaderResourceViewFromFile(device_, texture_noise, nullptr, nullptr, &noise_texture_, nullptr); // load noise texture

	load_texture(texture); // load diffuse texture
}


// draw method adding more resources and calling its base class draw
void LavaFloor::draw(const XMMATRIX view_projection, const bool use_default_cb, const D3D11_PRIMITIVE_TOPOLOGY mode) 
{
	immediate_context_->PSSetShaderResources(1, 1, &normal_texture_); // set normal texture as second shader resource 
	immediate_context_->PSSetShaderResources(2, 1, &noise_texture_); // set noise texture as third shader resource 
	Model::draw(view_projection, use_default_cb, mode); // call model draw
}

