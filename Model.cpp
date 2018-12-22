#include "Model.h"


// CONSTRUCTOR methods

Model::Model()
{
}

Model::Model(ID3D11Device * device, ID3D11DeviceContext * context, char * filename, CB_STATE state) : Model(device, context, state)
{
	LoadObjModel(filename);
}

Model::Model(ID3D11Device * device, ID3D11DeviceContext * context, CB_STATE state)
{
	this->device = device;
	this->immediateContext = context;
	this->state = state;
	switch (state)
	{
	case CB_STATE_FULL: CreateConstantBuffer_SIMPLE(); break;
	case CB_STATE_LIGHTED: CreateConstantBuffer_LIGHTED(); break;
	case CB_STATE_TIME_SCALED: CreateConstantBuffer_TIME_SCALED(); break;
	default: CreateConstantBuffer_SIMPLE(); break;
	}

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
	//CalculateOrigin();
	//InitializeCollider();
#ifdef RELEASE
	//OutputDebugString("Calc origin");
	CalculateOrigin();
	InitializeCollider();
#endif

}

// loads primitive topology shapes using indices
HRESULT Model::LoadGeoModel(void* vertices, UINT numverts, UINT single_vertex_bytesize, unsigned int *indices, UINT numIndices)
{
	HRESULT hr = S_OK;

	LoadGeoModel(vertices, numverts, single_vertex_bytesize);
	this->indices = indices;
	this->numIndices = numIndices;
	CreateIndexBuffer();

	return hr;
}

// loads primitive topology shapes using only vertices
HRESULT Model::LoadGeoModel(void* vertices, UINT numverts, UINT single_vertex_bytesize)
{
	HRESULT hr = S_OK;
	vertSize = single_vertex_bytesize;
	this->vertices = vertices;
	this->numverts = numverts;
	ZeroMemory(&numIndices, sizeof(numIndices));
	UpdateDefaultVertexBuffer(vertices, vertSize  * numverts);

#ifdef RELEASE
	//OutputDebugString("Calc origin");
	CalculateOrigin();
	InitializeCollider();
#endif

	return hr;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// DRAW

void Model::Draw(XMMATRIX view_projection, bool use_simple_cb, D3D11_PRIMITIVE_TOPOLOGY mode)
{
	if (use_simple_cb) {
		cb_simple.WorldViewProjection = view_projection;
		immediateContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb_simple, 0, 0);
	}
	else
	{
		//cb_time_scaled_lighted.WorldViewProjection = view_projection;
		immediateContext->UpdateSubresource(constantBuffer, 0, nullptr, get_constant_buffer_state(), 0, 0);
	}

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
	}
	else
	{
		immediateContext->Draw(numverts, 0);
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////



// INITIALIZATION methods


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
	SetDefaultInputLayout();
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

void Model::UpdateConstantBuffer_FULL(XMMATRIX world_view_projection, XMMATRIX view_projection, XMVECTOR directional_light_vector,
	XMVECTOR directional_light_color, XMVECTOR ambient_light_color, XMVECTOR rgb_amount, float gameTime)
{
	cb_full.WorldViewProjection = world_view_projection;
	cb_full.ViewProjection = view_projection;
	cb_full.directional_light_vector = directional_light_vector;
	cb_full.directional_light_colour = directional_light_color;
	cb_full.ambient_light_colour = ambient_light_color;
	cb_full.rgb_amount = rgb_amount;
	cb_full.gameTime = gameTime;

	cb = &cb_full;
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

void Model::UpdateConstantBuffer_TIME_SCALED(XMMATRIX world_view_projection, XMMATRIX view_projection, XMVECTOR directional_light_vector,
	XMVECTOR directional_light_color, XMVECTOR ambient_light_color, float gameTime)
{
	cb_time_scaled_lighted.WorldViewProjection = world_view_projection;
	cb_time_scaled_lighted.ViewProjection = view_projection;
	cb_time_scaled_lighted.directional_light_vector = directional_light_vector;
	cb_time_scaled_lighted.directional_light_colour = directional_light_color;
	cb_time_scaled_lighted.ambient_light_colour = ambient_light_color;
	cb_time_scaled_lighted.gameTime = gameTime;

	cb = &cb_time_scaled_lighted;
}

void Model::UpdateConstantBuffer_LIGHTED(XMMATRIX world_view_projection, XMVECTOR directional_light_vector,
	XMVECTOR directional_light_color, XMVECTOR ambient_light_color)
{
	cb_lighted.WorldViewProjection = world_view_projection;
	cb_lighted.directional_light_vector = directional_light_vector;
	cb_lighted.directional_light_colour = directional_light_color;
	cb_lighted.ambient_light_colour = ambient_light_color;
}

SphereCollider Model::getCollisionSphere() const
{
	return sphereCollider;
}

void Model::Update()
{
	
}

void Model::CalculateOrigin()
{

	vector<XMVECTOR> positions = objFileModel->getVertexPositions();
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

HRESULT Model::CreateConstantBuffer_FULL()
{
	cb = &cb_full;
	return CreateConstantBuffer(sizeof(cb_full));
}

HRESULT Model::CreateConstantBuffer_TIME_SCALED()
{
	cb = &cb_time_scaled_lighted;
	return CreateConstantBuffer(sizeof(cb_time_scaled_lighted));
}

HRESULT Model::CreateConstantBuffer_LIGHTED()
{
	cb = &cb_lighted;
	return CreateConstantBuffer(sizeof(cb_lighted));
}

HRESULT Model::CreateConstantBuffer_SIMPLE()
{
	cb = &cb_simple;
	return CreateConstantBuffer(sizeof(cb_simple));
}

HRESULT Model::CreateConstantBuffer(UINT bytewidth)
{
	HRESULT hr = NULL;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_DEFAULT; // can use UpdateSubresrource() to update
	desc.ByteWidth = bytewidth;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = device->CreateBuffer(&desc, NULL, &constantBuffer);

	return hr;
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

void* Model::get_constant_buffer_state()
{
	switch (state)
	{
	case CB_STATE_FULL: return &cb_full; break;
	case CB_STATE_LIGHTED: return &cb_lighted;; break;
	case CB_STATE_TIME_SCALED:return &cb_time_scaled_lighted;; break;
	default: return &cb_simple;; break;
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

