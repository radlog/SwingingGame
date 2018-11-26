#pragma once
#include "objfilemodel.h"
#include "Transform.h"

struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection;
};

struct SphereCollider
{
	XMVECTOR localPosition;
	float collisionRadius;
};

class Model
{
public:
	Model();
	Model(ID3D11Device* device, ID3D11DeviceContext* context, char * filename);
	
	Model(ID3D11Device* device, ID3D11DeviceContext* context);
	~Model();

	HRESULT LoadObjModel(char* filename);
	HRESULT CompileShaders();
	HRESULT SetDefaultInputLayout();
	HRESULT SetInputLayout(D3D11_INPUT_ELEMENT_DESC iedesc[], int size);
	void set_shader_file(char* shader_file);
	void Draw(XMMATRIX view_projection, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	HRESULT CreateDefaultConstantBuffer();
	HRESULT CreateDefaultSamplerForTexture();
	HRESULT LoadTexture();

	void UpdateConstantBufferValues();

	void Cleanup() const;
private:
	MODEL_CONSTANT_BUFFER cb;
	string shader_file = "model_shaders.hlsl";
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;

	ObjFileModel* objFileModel;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3DBlob *VS, *PS;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* constantBuffer;

	ID3D11SamplerState * sampler0;
	ID3D11ShaderResourceView *texture;

	Transform transform;

	SphereCollider sphereCollider;
	XMVECTOR minOuterVector;
	XMVECTOR maxOuterVector;
	XMVECTOR origin;

	void CalculateOrigin();
	void InitializeCollider();
	bool recalcOrigin = false;
};

