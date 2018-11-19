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
	XMVECTOR up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMVECTOR down = XMVectorSet(0.0, -1.0, 0.0, 0.0);
	XMVECTOR left = XMVectorSet(-1.0, 0.0, 0.0, 0.0);
	XMVECTOR right = XMVectorSet(1.0, 1.0, 0.0, 0.0);
	XMVECTOR forward = XMVectorSet(0.0, 0.0, 1.0, 0.0);
	XMVECTOR backward = XMVectorSet(0.0, 0.0, -1.0, 0.0);

	XMVECTOR scale;
	XMVECTOR rotation;
	XMVECTOR position;
	XMMATRIX world; 

	void translate(XMVECTOR position);
	void rotate(float yaw, float pitch, float roll);
	//void scale(float x, float y, float z);

protected:


private:



};

