#include "Model.h"



Model::Model()
{
}

Model::Model(ID3D11Device * device, ID3D11DeviceContext * context, char * filename) : Model(device,context)
{	
	LoadObjModel(filename);
}

Model::Model(ID3D11Device * device, ID3D11DeviceContext * context)
{
	this->device = device;
	this->immediateContext = context;
	CreateConstantBuffer();
	CompileShaders();
	SetDefaultInputLayout();
}

Model::~Model()
{
}

HRESULT Model::LoadObjModel(char * filename)
{
	objFileModel = new ObjFileModel(filename,device,immediateContext);
	
	if(objFileModel->filename == "FILE NOT LOADED") return S_FALSE;
}

HRESULT Model::CompileShaders()
{
	HRESULT hr;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *error;
	hr = D3DX11CompileFromFile(shader_file.c_str(), 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	hr = D3DX11CompileFromFile(shader_file.c_str(), 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	// create shader objects
	hr = device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &vShader);
	hr = device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pShader);



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return hr;
}

HRESULT Model::SetDefaultInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	return SetInputLayout(iedesc, ARRAYSIZE(iedesc));
}

HRESULT Model::SetInputLayout(D3D11_INPUT_ELEMENT_DESC iedesc[], int size)
{
	HRESULT hr;
	hr = device->CreateInputLayout(iedesc, size, VS->GetBufferPointer(), VS->GetBufferSize(), &inputLayout);
	immediateContext->IASetInputLayout(inputLayout);
	return hr;
}

void Model::set_shader_file(char * shader_file)
{
	this->shader_file = shader_file;
}

void Model::Draw(XMMATRIX view_projection)
{
	XMMATRIX world = XMMatrixMultiply( 
		XMMatrixScalingFromVector(XMVECTOR(XMVectorSet(1.0f,1.0f,1.0f,0.0f))), 
		XMMatrixRotationQuaternion( XMQuaternionIdentity())) 
		* XMMatrixIdentity();

	cb.WorldViewProjection = view_projection;

	immediateContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);
	immediateContext->VSSetConstantBuffers(0,1,&constantBuffer);

	// set the shader objects as active
	immediateContext->VSSetShader(vShader, 0, 0);
	immediateContext->PSSetShader(pShader, 0, 0);

	objFileModel->Draw();
}

HRESULT Model::CreateConstantBuffer()
{
	HRESULT hr;
	// create constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // can use UpdateSubresrource() to update
	constant_buffer_desc.ByteWidth = sizeof(cb); // MUST be a multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = device->CreateBuffer(&constant_buffer_desc, NULL, &constantBuffer);

	return hr;
}

