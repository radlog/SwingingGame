#pragma once
#include "ObjFileModel.h"
#include "Geometry.h"

enum GeoTypes
{

};

class GeoModel
{
public:
	GeoModel();
	GeoModel(ID3D11Device* device, ID3D11DeviceContext* context);
	~GeoModel();
	void Draw();
private:
	ID3D11Device*           device;
	ID3D11DeviceContext*    immediateContext;
	ID3D11Buffer* vertexBuffer;	

	HRESULT UpdateVertexBuffer();
};

