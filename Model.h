#pragma once
#include "objfilemodel.h"
#include "Transform.h"

struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection;
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
	void Draw(XMMATRIX view_projection);
	HRESULT CreateConstantBuffer();

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

	Transform transform;

};

