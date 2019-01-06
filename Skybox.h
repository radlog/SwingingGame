#pragma once

class Skybox : public GameObject
{
public:
	~Skybox();
	Skybox();

	explicit Skybox(LPCSTR texture_name);
	Skybox(LPCSTR name, Transform transform, Model model);
	void draw(XMMATRIX view_projection, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) const;

private:
	ID3D11ShaderResourceView *texture_;

	ID3D11RasterizerState *rasterizer_solid_ = nullptr;
	ID3D11RasterizerState *rasterizer_sky_ = nullptr;
	ID3D11DepthStencilState *depth_write_solid_ = nullptr;
	ID3D11DepthStencilState *depth_write_sky_ = nullptr;

	HRESULT init_rasterizer();
	HRESULT init_depth_stencil_states();
	void cleanup() const;
public:
	void spawn(XMVECTOR position) override;
	void render() override;
	void start() override;
	void update(VGTime timer) override;
	void cleanup() override;
};

