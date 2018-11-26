#include "pch.h"
#include "Floor.h"


Floor::Floor()
{
}


Floor::~Floor()
{
}


Floor::Floor(ID3D11Device * device, ID3D11DeviceContext * context) : Model(device, context)
{
	LoadObjModel(filename);
}