#pragma once
#include "BaseModel.h"
#include "Collider.h"

class Model :
	public BaseModel
{
public:
	explicit Model(LPCSTR filename, CB_STATE state = CB_STATE_SIMPLE);
	explicit Model(CB_STATE state = CB_STATE_SIMPLE);
	~Model();

	void draw(XMMATRIX view_projection, bool use_simple_cb, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;

	
protected:

};

