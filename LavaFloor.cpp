#include "pch.h"
#include "LavaFloor.h"


LavaFloor::LavaFloor()
{
}


LavaFloor::~LavaFloor()
{


}

LavaFloor::LavaFloor(const LPCSTR texture, const LPCSTR texture_normal, const LPCSTR texture_noise, const int tiles, const float scale) : GameObject(texture)
{
	POS_TEX_NORM_COL_VERTEX *plane_vertices;
	unsigned int *plane_indices;

	model_ = Model(dx_handle_->device, dx_handle_->immediate_context, CB_STATE_TIME_SCALED);
	Geometry::create_indexed_tiled_textured_normal_plane(&plane_vertices, &plane_indices, tiles, scale);
	model_.LoadGeoModel(plane_vertices, (tiles + 1)*(tiles + 1), sizeof(POS_TEX_NORM_COL_VERTEX), plane_indices, tiles * tiles * 6);
	char lava_shader[] = "lava_shader.hlsl";
	model_.set_shader_file(lava_shader);

	D3DX11CreateShaderResourceViewFromFile(device_, texture_normal, nullptr, nullptr, &normal_texture_, nullptr);
	D3DX11CreateShaderResourceViewFromFile(device_, texture_noise, nullptr, nullptr, &noise_texture_, nullptr);


	model_.LoadTexture("assets/lava_selfmade_diffuse.png");
	transform = Transform(XMVectorSet(scale, 1.0f, scale, 0.0f), XMQuaternionIdentity(), XMVectorSet((-tiles * scale) / 2, -10.0f, (-tiles * scale) / 2, 0.0f));

}



void LavaFloor::draw(const XMMATRIX view_projection, const bool use_default_cb, const D3D11_PRIMITIVE_TOPOLOGY mode) 
{
	immediate_context_->PSSetShaderResources(1, 1, &normal_texture_);
	immediate_context_->PSSetShaderResources(2, 1, &noise_texture_);
	GameObject::draw(view_projection, use_default_cb, mode);
}

void LavaFloor::spawn(XMVECTOR position)
{
}

void LavaFloor::render()
{
}

void LavaFloor::start()
{
}

void LavaFloor::update(VGTime timer)
{
}

void LavaFloor::cleanup()
{
}
