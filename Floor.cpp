#include "Floor.h"


Floor::Floor()
{
}


Floor::~Floor()
{
}


Floor::Floor(ID3D11Device * device, ID3D11DeviceContext * context, CB_STATE state) : Model(device, context,state)
{
	LoadObjModel(filename);
}