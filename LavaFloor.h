#pragma once
#include "GameObject.h"
class LavaFloor : public GameObject
{
public:
	LavaFloor();
	~LavaFloor();
	explicit LavaFloor(LPCSTR texture, LPCSTR texture_normal = "assets/lava_selfmade_normal.png", LPCSTR texture_noise= "assets/lava_noise.png", int tiles = 200,float scale = 1.0f);


	void draw(XMMATRIX view_projection, bool use_default_cb = false, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	ID3D11ShaderResourceView *noise_texture_;
	ID3D11ShaderResourceView *normal_texture_;
};

