#pragma once
#include "GameObject.h"

class Floor : public Model
{
public:
	Floor();
	~Floor();
	Floor(ID3D11Device* device, ID3D11DeviceContext* context, char* filename, CB_STATE state);
	Floor(ID3D11Device * device, ID3D11DeviceContext * context, CB_STATE state);


private:
	char* filename;// = "floor.obj";
};

