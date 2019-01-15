#pragma once
class ParticleFactory : public Model
{

	struct Particle
	{
		float gravity;
		XMFLOAT3 position;
		XMFLOAT3 velocity;
		XMFLOAT4 color;
	};

public:
	ParticleFactory();
	~ParticleFactory();

	int create_particle();
	void draw(const XMMATRIX* view_projection, D3D_PRIMITIVE_TOPOLOGY mode = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void draw_one(Particle* one, const XMMATRIX* view_projection, D3D_PRIMITIVE_TOPOLOGY mode = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
private:
	ID3D11Buffer* vertex_buffer_;
	ID3D11RasterizerState *rasterizer_solid_ = nullptr;
	ID3D11RasterizerState *rasterizer_particle_ = nullptr;
	CONSTANT_BUFFER_PARTICLE cb_particle_;
	HRESULT create_particle_buffer();
	void update_particle_buffer(XMMATRIX world_view_projection, XMFLOAT4 color);
};

