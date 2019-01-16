#pragma once

class Skybox : public GameObject
{
public:
	~Skybox();
	Skybox();

	explicit Skybox(LPCSTR texture_name);
	void draw(XMMATRIX view_projection, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) const;
	void cleanup() const;

private:

	ID3D11RasterizerState *rasterizer_solid_ = nullptr; // solid rasterizer state
	ID3D11RasterizerState *rasterizer_sky_ = nullptr; // sky rasterizer state
	ID3D11DepthStencilState *depth_write_solid_ = nullptr; // solid depth stencil state
	ID3D11DepthStencilState *depth_write_sky_ = nullptr; // sky depth stencil state

	HRESULT init_rasterizer(); // initialize rasterizer
	HRESULT init_depth_stencil_states(); // initialize depth stencil state


	
};

