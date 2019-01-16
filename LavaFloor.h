#pragma once
#include "Floor.h"

class LavaFloor : public Floor
{
public:
	LavaFloor();
	~LavaFloor();
	// lava floor initializer taking texture, tiles, scale, normal texture, noise texture
	explicit LavaFloor(LPCSTR texture, int tiles = 200,float scale = 1.0f, LPCSTR texture_normal = "assets/lava_selfmade_normal.png", LPCSTR texture_noise = "assets/lava_noise.png");

	void draw(XMMATRIX view_projection, bool use_default_cb = false, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override; // draw method override 

private:
	ID3D11ShaderResourceView *noise_texture_; // noise texture
	ID3D11ShaderResourceView *normal_texture_; // normal texture
};

