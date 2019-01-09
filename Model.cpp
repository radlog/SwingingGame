#include "pch.h"
#include "Model.h"




Model::Model(LPCSTR filename, CB_STATE state) : BaseModel(filename, state)
{
}

Model::Model(CB_STATE state) : BaseModel(state)
{

}

Model::~Model()
{
}

void Model::draw(const XMMATRIX view_projection, const bool use_simple_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	BaseModel::draw(view_projection, use_simple_cb, mode);
}

