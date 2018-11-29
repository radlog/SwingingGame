#pragma once
class Skybox : public GameObject
{
public:
	Skybox();
	~Skybox();

	Skybox(std::string name);
	Skybox(std::string name, Transform transform, Model model);

private:
	ID3D11ShaderResourceView *texture;

	ID3D11RasterizerState *rasterizerSolid = 0;
	ID3D11RasterizerState *rasterizerSky = 0;
	ID3D11DepthStencilState *depthWriteSolid = 0;
	ID3D11DepthStencilState *depthWriteSky = 0;
};

