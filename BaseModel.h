#pragma once
#include "objfilemodel.h"
#include "BufferDefines.h"
#include "d3dcompiler.h"

class D3Dfw;



enum CB_STATE
{
	CB_STATE_SIMPLE = 0,
	CB_STATE_LIGHTED = 1,
	CB_STATE_TIME_SCALED = 2,
	CB_STATE_FULL = 3
};

class BaseModel
{
public:
	explicit BaseModel(LPCSTR filename, CB_STATE state = CB_STATE_TIME_SCALED); // initialize a basic model object with filename (path included), state of constant buffer
	explicit BaseModel(CB_STATE state = CB_STATE_TIME_SCALED); // initialize a model without existing file that uses the geometry class that adds vertices to draw
	virtual ~BaseModel();

	HRESULT load_obj_model(LPCSTR filename); // loads a model by filename (path included)
	HRESULT load_geo_model(void* vertices, UINT num_verts, UINT single_vertex_bytesize, unsigned int *indices, UINT
	                       num_indices);
	HRESULT load_geo_model(void* vertices, const UINT numverts, const UINT single_vertex_bytesize);

	void set_shader_file(LPCSTR shader_file);

	virtual void draw(XMMATRIX view_projection, bool use_simple_cb, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void cleanup() const;

	HRESULT load_texture(LPCSTR filename = "assets/crate.jpg");
	HRESULT load_normal(LPCSTR filename);

	void update_constant_buffer_full(XMMATRIX world_view_projection,XMMATRIX view_projection, XMVECTOR directional_light_vector, XMVECTOR directional_light_color, XMVECTOR ambient_light_color, XMVECTOR rgb_amount, float gameTime);
	void update_constant_buffer_time_scaled(XMMATRIX world_view_projection, XMMATRIX view_projection, XMVECTOR directional_light_vector, XMVECTOR directional_light_color, XMVECTOR ambient_light_color, float
	                                        game_time);
	void update_constant_buffer_lighted(XMMATRIX world_view_projection, const XMMATRIX view_projection, XMVECTOR directional_light_vector, XMVECTOR directional_light_color, XMVECTOR ambient_light_color);
	//SphereCollider get_collision_sphere() const;
	void update() const;


protected:
	ObjFileModel *obj_file_model_;

	// different constant buffer defines
	void *cb_;
	CONSTANT_BUFFER_SIMPLE cb_simple_; 
	CONSTANT_BUFFER_LIGHTED cb_lighted_;
	CONSTANT_BUFFER_TIME_SCALED cb_time_scaled_lighted_;
	CONSTANT_BUFFER_FULL cb_full_;

	string shader_file_ = "lighted_shader.hlsl"; // name of the shader file to load
	string objfile_; // name of the object file to load

	D3Dfw* dx_handle_; // directX instance handle
	ID3D11Device *device_; // hardware device
	ID3D11DeviceContext *immediate_context_; // device context

	ID3D11VertexShader *v_shader_; // vertex shader pointer
	ID3D11PixelShader *p_shader_; // pixel shader pointer
	ID3DBlob *vs_, *ps_; // blob resources for vertex and pixel shader
	ID3D11InputLayout *input_layout_; // input layout to pass into the shader
	ID3D11Buffer *constant_buffer_; // the constant buffer to use for drawing
	ID3D11Buffer *vertex_buffer_; // vertex buffer holding vertex data
	ID3D11Buffer *index_buffer_; // index buffer for the vertices

	ID3D11SamplerState *sampler0_; // sampler state for the texture
	ID3D11ShaderResourceView *texture_; // texture to draw
	ID3D11ShaderResourceView *normal_; // normal texture to draw

	XMVECTOR min_outer_vector_; // farthest minimum outer vector of the model
	XMVECTOR max_outer_vector_; // farthest maximum outer vector of the model
	XMVECTOR origin_; // origin to be calculated using outer vectors

	UINT numverts_; // number of vertices
	UINT vert_size_; // vertex byte size
	void *vertices_; // this is a void pointer, because a vertex can have different types of structures
	UINT num_indices_; // number of indices
	unsigned int *indices_; // indices

	void calculate_origin(); // calculates the origin of the model in local space

	CB_STATE state_; // constant buffer state used to define which type of constant buffer to use

	// methods to create different constant buffers
	HRESULT create_constant_buffer_full(); 
	HRESULT create_constant_buffer_time_scaled();
	HRESULT create_constant_buffer_lighted();
	HRESULT create_constant_buffer_simple();
	HRESULT create_constant_buffer(UINT byte_width);

	HRESULT compile_shaders(); // compile shader file
	HRESULT set_default_input_layout(); // set default input layout
	HRESULT set_input_layout(D3D11_INPUT_ELEMENT_DESC iedesc[], int size); // set input layout
	HRESULT create_default_sampler_for_texture(); // create a single sampler for the texture of this object
	HRESULT update_default_vertex_buffer(void* vertices, UINT byte_width); // update vertex buffer
	HRESULT create_index_buffer(); // create index buffer

	void* get_constant_buffer_state(); // returns constant buffer depending on which CB_STAT currently is active
};
