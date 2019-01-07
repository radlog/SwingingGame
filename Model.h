#pragma once
#include "objfilemodel.h"
#include "BufferDefines.h"

class D3Dfw;

struct SphereCollider
{
	XMVECTOR local_position;
	float collision_radius;
};

enum CB_STATE
{
	CB_STATE_SIMPLE = 0,
	CB_STATE_LIGHTED = 1,
	CB_STATE_TIME_SCALED = 2,
	CB_STATE_FULL = 3
};

class Model
{
public:
	explicit Model(LPCSTR filename, CB_STATE state = CB_STATE_SIMPLE);
	explicit Model(CB_STATE state = CB_STATE_SIMPLE);
	~Model();

	HRESULT load_obj_model(LPCSTR filename);
	HRESULT load_geo_model(void* vertices, UINT num_verts, UINT single_vertex_bytesize, unsigned int *indices, UINT
	                       num_indices);
	HRESULT load_geo_model(void* vertices, UINT numverts, UINT single_vertex_bytesize);

	void set_shader_file(char* shader_file);

	void draw(XMMATRIX view_projection, bool use_simple_cb, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void cleanup() const;

	HRESULT load_texture(LPCSTR filename = "assets/crate.jpg");
	HRESULT load_normal(LPCSTR filename);

	void update_constant_buffer_full(XMMATRIX world_view_projection,XMMATRIX view_projection, XMVECTOR directional_light_vector, XMVECTOR directional_light_color, XMVECTOR ambient_light_color, XMVECTOR rgb_amount, float gameTime);
	void update_constant_buffer_time_scaled(XMMATRIX world_view_projection, XMMATRIX view_projection, XMVECTOR directional_light_vector, XMVECTOR directional_light_color, XMVECTOR ambient_light_color, float
	                                        game_time);
	void update_constant_buffer_lighted(XMMATRIX world_view_projection, const XMMATRIX view_projection, XMVECTOR directional_light_vector, XMVECTOR directional_light_color, XMVECTOR ambient_light_color);
	SphereCollider get_collision_sphere() const;
	void update() const;


protected:
	ObjFileModel *obj_file_model_;

	void *cb_;
	CONSTANT_BUFFER_SIMPLE cb_simple_;
	CONSTANT_BUFFER_LIGHTED cb_lighted_;
	CONSTANT_BUFFER_TIME_SCALED cb_time_scaled_lighted_;
	CONSTANT_BUFFER_FULL cb_full_;
	string shader_file_ = "default_shader.hlsl";
	string objfile_;

	D3Dfw* dx_handle_; // directX instance handle
	ID3D11Device *device_; // hardware device
	ID3D11DeviceContext *immediate_context_; // device context


	ID3D11VertexShader *v_shader_;
	ID3D11PixelShader *p_shader_;
	ID3DBlob *vs_, *ps_;
	ID3D11InputLayout *input_layout_;
	ID3D11Buffer *constant_buffer_;
	ID3D11Buffer *vertex_buffer_;
	ID3D11Buffer *index_buffer_;

	ID3D11SamplerState *sampler0_;
	ID3D11ShaderResourceView *texture_;
	ID3D11ShaderResourceView *normal_;

	SphereCollider sphere_collider_;
	XMVECTOR min_outer_vector_;
	XMVECTOR max_outer_vector_;
	XMVECTOR origin_;

	UINT numverts_;
	UINT vert_size_;
	void *vertices_;
	UINT num_indices_;
	unsigned int *indices_;

	void calculate_origin();
	void initialize_collider();

	bool recalc_origin_ = false;

	CB_STATE state_;

	HRESULT create_constant_buffer_full();
	HRESULT create_constant_buffer_time_scaled();
	HRESULT create_constant_buffer_lighted();
	HRESULT create_constant_buffer_simple();
	HRESULT create_constant_buffer(UINT byte_width);

	HRESULT compile_shaders();
	HRESULT set_default_input_layout();
	HRESULT set_input_layout(D3D11_INPUT_ELEMENT_DESC iedesc[], int size);
	HRESULT create_default_sampler_for_texture();
	HRESULT update_default_vertex_buffer(void* vertices, UINT byte_width);
	HRESULT create_index_buffer();

	void* get_constant_buffer_state();
};
