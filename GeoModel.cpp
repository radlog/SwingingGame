#include "pch.h"
#include "GeoModel.h"


GeoModel::GeoModel(ID3D11Device* device, ID3D11DeviceContext* context)
{
	this->device = device;
	immediateContext = context;
}


GeoModel::~GeoModel()
{
}

//HRESULT GeoModel::UpdateVertexBuffer()
//{
//	// Set up and create vertex buffer
//	D3D11_BUFFER_DESC bufferDesc;
//	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
//	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	bufferDesc.ByteWidth = sizeof(vertices[0])*numverts;
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	HRESULT hr = device->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);
//
//	if (FAILED(hr)) return hr;
//	
//
//	
//	D3D11_MAPPED_SUBRESOURCE ms;
//	immediateContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
//	memcpy(ms.pData, vertices, sizeof(vertices[0])*numverts);
//	immediateContext->Unmap(vertexBuffer, NULL);
//
//
//	return hr;
//}
//
//
//void GeoModel::Draw(void)
//{
//	UINT stride = sizeof(MODEL_POS_TEX_NORM_VERTEX);
//	UINT offset = 0;
//	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
//	immediateContext->Draw(numverts, 0);
//}
