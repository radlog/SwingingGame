#pragma once
class Skybox : public GameObject
{
public:
	~Skybox();
	Skybox();

	Skybox(LPCSTR texturename);
	Skybox(LPCSTR name, Transform transform, Model model);
	void Draw(XMMATRIX view_projection, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	ID3D11ShaderResourceView *texture;

	ID3D11RasterizerState *rasterizerSolid = 0;
	ID3D11RasterizerState *rasterizerSky = 0;
	ID3D11DepthStencilState *depthWriteSolid = 0;
	ID3D11DepthStencilState *depthWriteSky = 0;

	HRESULT InitRasterizer();
	HRESULT InitDepthStencilStates();
	void Cleanup();
};

