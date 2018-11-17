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

	XMVECTOR Forward;
	XMVECTOR Backward;
	XMVECTOR Left;
	XMVECTOR Right;
	XMVECTOR Up;
	XMVECTOR Down;

	XMVECTOR position;
	XMVECTOR scale;
	XMVECTOR rotation;
	XMVECTOR lookat;


	void translate(XMMATRIX position);
	void rotate(float yaw, float pitch, float roll);

};





