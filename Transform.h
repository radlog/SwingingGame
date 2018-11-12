#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include "Vector3.h"
class Transform
{
public:
	Transform();
	~Transform();

	XMMATRIX get_world_transform();
	Vector3 Forward;
	Vector3 Backward;
	Vector3 Left;
	Vector3 Right;
	Vector3 Up;
	Vector3 Down;

	void translate(Vector3 position);
	void rotate(float yaw, float pitch, float roll);
	void scale(float x, float y, float z);

protected:


private:
	XMMATRIX rotation, translation, scale, world;
	Vector3 position;
	Vector3  scale;
	//Quaternion rotation;


};

