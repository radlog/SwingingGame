#pragma once
#include "objfilemodel.h"
#include "BufferDefines.h"

struct SphereCollider
{
	XMVECTOR localPosition;
	float collisionRadius;
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
	Model();
	Model(ID3D11Device* device, ID3D11DeviceContext* context, char * filename, CB_STATE state = CB_STATE_SIMPLE);

	Model(ID3D11Device* device, ID3D11DeviceContext* context, CB_STATE state = CB_STATE_SIMPLE);
	~Model();

	HRESULT LoadObjModel(char* filename);
	HRESULT LoadGeoModel(void* vertices, UINT numverts, UINT single_vertex_bytesize, unsigned int *indices, UINT numIndices);
	HRESULT LoadGeoModel(void* vertices, UINT numverts, UINT single_vertex_bytesize);

	void set_shader_file(char* shader_file);

	void Draw(XMMATRIX view_projection, bool use_simple_cb, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void Cleanup() const;

	HRESULT LoadTexture(LPCSTR filename = "assets/crate.jpg");
	HRESULT LoadNormal(LPCSTR filename);

	void UpdateConstantBuffer_FULL(XMMATRIX world_view_projection,XMMATRIX view_projection, XMVECTOR directional_light_vector, XMVECTOR directional_light_color, XMVECTOR ambient_light_color, XMVECTOR rgb_amount, float gameTime);
	void UpdateConstantBuffer_TIME_SCALED(XMMATRIX world_view_projection, XMMATRIX view_projection, XMVECTOR directional_light_vector, XMVECTOR directional_light_color, XMVECTOR ambient_light_color, float gameTime);
	void UpdateConstantBuffer_LIGHTED(XMMATRIX world_view_projection, XMVECTOR directional_light_vector, XMVECTOR directional_light_color, XMVECTOR ambient_light_color);
	SphereCollider getCollisionSphere() const;
	void Update();


protected:
	ObjFileModel* objFileModel;

	void *cb;
	CONSTANT_BUFFER_SIMPLE cb_simple;
	CONSTANT_BUFFER_LIGHTED cb_lighted;
	CONSTANT_BUFFER_TIME_SCALED cb_time_scaled_lighted;
	CONSTANT_BUFFER_FULL cb_full;
	string shader_file = "default_shader.hlsl";
	string objfile;
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;


	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3DBlob *VS, *PS;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* constantBuffer;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11SamplerState * sampler0;
	ID3D11ShaderResourceView *texture;

	SphereCollider sphereCollider;
	XMVECTOR minOuterVector;
	XMVECTOR maxOuterVector;
	XMVECTOR origin;

	UINT numverts;
	UINT vertSize;
	void *vertices;
	UINT numIndices;
	unsigned int *indices;

	void CalculateOrigin();
	void InitializeCollider();

	bool recalcOrigin = false;

	CB_STATE state;

	HRESULT CreateConstantBuffer_FULL();
	HRESULT CreateConstantBuffer_TIME_SCALED();
	HRESULT CreateConstantBuffer_LIGHTED();
	HRESULT CreateConstantBuffer_SIMPLE();
	HRESULT CreateConstantBuffer(UINT bytewidth);

	HRESULT CompileShaders();
	HRESULT SetDefaultInputLayout();
	HRESULT SetInputLayout(D3D11_INPUT_ELEMENT_DESC iedesc[], int size);
	HRESULT CreateDefaultSamplerForTexture();
	HRESULT UpdateDefaultVertexBuffer(void* vertices, UINT byteWidth);
	HRESULT CreateIndexBuffer();

	void* get_constant_buffer_state();
};
