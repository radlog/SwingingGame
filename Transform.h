#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <xnamath.h>
class Transform
{
public:
	Transform();
	~Transform();

	XMMATRIX get_world_transform();

protected:


private:
	XMMATRIX rotation, translation, scale, world;

};

