#pragma once
#include "GameObject.h"

class Floor : public Model
{
public:
	Floor();
	~Floor();
	Floor(ID3D11Device* device, ID3D11DeviceContext* context, char* filename);
	Floor(ID3D11Device* device, ID3D11DeviceContext* context);


private:
	char* filename;// = "floor.obj";
};

