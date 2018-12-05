#pragma once
#include "GameObject.h"
class LavaFloor : public GameObject
{
public:
	LavaFloor();
	~LavaFloor();
	LavaFloor(LPCSTR texture, LPCSTR texture_normal, int tiles = 200,float scale = 1.0f);

	LavaFloor(LPCSTR name);
	LavaFloor(LPCSTR name, Transform transform, Model model);


	void Draw(XMMATRIX view_projection, bool use_default_cb = false, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	ID3D11ShaderResourceView *noise_texture;
};

