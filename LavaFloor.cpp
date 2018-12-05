#include "pch.h"
#include "LavaFloor.h"


LavaFloor::LavaFloor()
{
}


LavaFloor::~LavaFloor()
{


}

LavaFloor::LavaFloor(LPCSTR texture,LPCSTR texture_normal, LPCSTR texture_noise, int tiles, float scale) : GameObject(texture)
{
	POS_TEX_NORM_COL_VERTEX *plane_vertices;
	unsigned int *plane_indices;

	model = Model(dx_handle->device, dx_handle->immediateContext, CB_STATE_TIME_SCALED);
	Geometry::create_indexed_tiled_textured_normal_plane(&plane_vertices, &plane_indices, tiles, scale);
	model.LoadGeoModel(plane_vertices, (tiles + 1)*(tiles + 1), sizeof(POS_TEX_NORM_COL_VERTEX), plane_indices, tiles * tiles * 6);
	char lava_shader[] = "lava_shader.hlsl";
	model.set_shader_file(lava_shader);

	D3DX11CreateShaderResourceViewFromFile(device, texture_normal, nullptr, nullptr, &normal_texture, nullptr);
	D3DX11CreateShaderResourceViewFromFile(device, texture_noise, nullptr, nullptr, &noise_texture, nullptr);


	model.LoadTexture("assets/lava_selfmade_diffuse.png");
	transform = Transform(XMVectorSet(scale, 1.0f, scale, 0.0f), XMQuaternionIdentity(), XMVectorSet((-tiles * scale) / 2, -10.0f, (-tiles * scale) / 2, 0.0f));

}



void LavaFloor::Draw(XMMATRIX view_projection, bool use_default_cb, D3D11_PRIMITIVE_TOPOLOGY mode) 
{
	immediateContext->PSSetShaderResources(1, 1, &normal_texture);
	immediateContext->PSSetShaderResources(2, 1, &noise_texture);
	GameObject::Draw(view_projection, use_default_cb, mode);
}
