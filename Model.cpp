#include "Model.h"
#include <minwinbase.h>


Model::Model()
{
}

Model::Model(ID3D11Device * device, ID3D11DeviceContext * context, char * filename) : Model(device, context)
{
	LoadObjModel(filename);
}

Model::Model(ID3D11Device * device, ID3D11DeviceContext * context)
{
	this->device = device;
	this->immediateContext = context;
	CreateDefaultConstantBuffer();
	CompileShaders();
	SetDefaultInputLayout();
	CreateDefaultSamplerForTexture();
	LoadTexture();
}

Model::~Model()
{
}

HRESULT Model::LoadObjModel(char * filename)
{
	objFileModel = new ObjFileModel(filename, device, immediateContext);
	objfile = filename;
	if (objFileModel->filename == "FILE NOT LOADED") return S_FALSE;

	numverts = objFileModel->numverts;
	vertices = objFileModel->vertices;
	vertSize = sizeof(objFileModel->vertices[0]);
	indices = objFileModel->indices;
	numIndices = numverts;

	UpdateDefaultVertexBuffer(vertices, vertSize  * numverts);
	CreateIndexBuffer();

#ifdef RELEASE
	//OutputDebugString("Calc origin");
	CalculateOrigin();
	InitializeCollider();
#endif

}

HRESULT Model::LoadGeoModel(void* vertices, UINT numverts, UINT single_vertex_bytesize, unsigned int *indices, UINT numIndices)
{
	HRESULT hr = S_OK;
	
	LoadGeoModel(vertices, numverts, single_vertex_bytesize);
	this->indices = indices;
	this->numIndices = numIndices;
	CreateIndexBuffer();

	return hr;
}


HRESULT Model::LoadGeoModel(void* vertices, UINT numverts, UINT single_vertex_bytesize)
{
	HRESULT hr = S_OK;
	vertSize = single_vertex_bytesize;
	this->vertices = vertices;
	this->numverts = numverts;
	UpdateDefaultVertexBuffer(vertices, vertSize  * numverts);

#ifdef RELEASE
	//OutputDebugString("Calc origin");
	CalculateOrigin();
	InitializeCollider();
#endif

	return hr;
}

HRESULT Model::CompileShaders()
{
	HRESULT hr;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *error;
	hr = D3DX11CompileFromFile(shader_file.c_str(), 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	hr = D3DX11CompileFromFile(shader_file.c_str(), 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

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
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0}
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
	CompileShaders();
	//UpdateModel();
}

HRESULT Model::CreateDefaultConstantBuffer()
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

HRESULT Model::CreateDefaultSamplerForTexture()
{
	HRESULT hr;

	D3D11_SAMPLER_DESC  sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sampler_desc, &sampler0);

	return hr;
}

HRESULT Model::LoadTexture(LPCSTR filename)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(device, filename, nullptr, nullptr, &texture, nullptr);
	return hr;
}

void Model::UpdateConstantBufferValues()
{
	//enable_glowing = true;
	//if (enable_glowing)
	//{
	//	static bool glow;
	//	delta *= 10;
	//	if (constantBuffer.RedAmount <= 0.01f || constantBuffer.GreenAmount <= 0.01f || constantBuffer.BlueAmount <= 0.01f) glow = true;
	//	else if (constantBuffer.RedAmount >= 0.99f || constantBuffer.GreenAmount >= 0.99f || constantBuffer.BlueAmount >= 0.99f) glow = false;

	//	if (glow)
	//	{
	//		constantBuffer.RedAmount += 0.1f * delta;
	//		constantBuffer.GreenAmount += 0.1f * delta;
	//		constantBuffer.BlueAmount += 0.1f * delta;
	//	}
	//	else
	//	{
	//		constantBuffer.RedAmount -= 0.1f * delta;
	//		constantBuffer.GreenAmount -= 0.1f * delta;
	//		constantBuffer.BlueAmount -= 0.1f * delta;
	//	}
	//}
	//else
	//{
	//	constantBuffer.RedAmount = 1.0f;
	//	constantBuffer.GreenAmount = 1.0f;
	//	constantBuffer.BlueAmount = 1.0f;
	//}

	//camera->look_at(XMVectorSet(0.0, 0.0, -4.0, 0.0));
	//const auto cube_rotation = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(timer->totalTime() * 4, timer->totalTime() * 2, timer->totalTime() * 3));
	//constantBuffer.WorldViewProjection = XMMatrixIdentity() * view_projection;

	//XMMATRIX transpose;
	//transpose = XMMatrixTranspose(constantBuffer.WorldViewProjection);

	//constantBuffer.directional_light_colour = directional_light_colour;
	//constantBuffer.ambient_light_colour = ambient_light_colour;
	//constantBuffer.directional_light_vector = XMVector3Normalize(XMVector3Transform(directional_light_shines_from, transpose));

}

void Model::Draw(XMMATRIX view_projection, D3D11_PRIMITIVE_TOPOLOGY mode)
{
	cb.WorldViewProjection = view_projection;

	immediateContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);
	immediateContext->VSSetConstantBuffers(0, 1, &constantBuffer);

	// set the shader objects as active
	immediateContext->VSSetShader(vShader, 0, 0);
	immediateContext->PSSetShader(pShader, 0, 0);

	immediateContext->PSSetSamplers(0, 1, &sampler0);
	immediateContext->PSSetShaderResources(0, 1, &texture);

	immediateContext->IASetPrimitiveTopology(mode);

	UINT stride = vertSize;
	UINT offset = 0;
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	if (numIndices)
	{
		immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		immediateContext->DrawIndexed(numIndices, 0, 0);
	}else
	{
		immediateContext->Draw(numverts, 0);		
	}

}

void Model::Cleanup() const
{
	if (device) device->Release();
	if (immediateContext) immediateContext->Release();
	if (vShader) vShader->Release();
	if (pShader)pShader->Release();
	if (VS) VS->Release();
	if (PS) PS->Release();
	if (inputLayout)inputLayout->Release();
	if (constantBuffer)constantBuffer->Release();
	if (sampler0)sampler0->Release();
	if (texture)texture->Release();
}

void Model::CalculateOrigin()
{
	vector<XMVECTOR> positions(objFileModel->numverts);
	objFileModel->getVertexPositions(positions);
	minOuterVector = positions[0];
	maxOuterVector = positions[1];

	float distance = 0;


	for (size_t i = 0; i < positions.size(); i++)
	{
		const auto a = positions[i];
		for (auto j = i; j < positions.size(); j++)
		{
			const auto b = positions[j];
			if (pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2) > distance)
			{
				minOuterVector = b;
				maxOuterVector = a;
				distance = pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2);
			}
		}
	}

	origin = minOuterVector - maxOuterVector;
}

void Model::InitializeCollider()
{
	sphereCollider.localPosition = origin;
	sphereCollider.collisionRadius = sqrt(pow(minOuterVector.x - maxOuterVector.x, 2) + pow(minOuterVector.y - maxOuterVector.y, 2) + pow(minOuterVector.z - maxOuterVector.z, 2));
}

HRESULT Model::UpdateDefaultVertexBuffer(void *vertices, UINT byteWidth)
{
	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr = device->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);

	if (FAILED(hr)) return hr;


	D3D11_MAPPED_SUBRESOURCE ms;
	immediateContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, byteWidth);
	immediateContext->Unmap(vertexBuffer, NULL);


	return hr;
}

HRESULT Model::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = numIndices * sizeof(unsigned int);
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&desc, &indexData, &indexBuffer);

	return hr;
}

HRESULT Model::UpdateModel()
{
	HRESULT hr = NULL;
	CreateDefaultConstantBuffer();
	CompileShaders();
	SetDefaultInputLayout();
	CreateDefaultSamplerForTexture();
	LoadTexture();
	return hr;
}
