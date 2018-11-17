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

<<<<<<< HEAD
=======
private:
	XMMATRIX XM_rotation, XM_translation, XM_scale, XM_world;
	Vector3 position;
	//Vector3 scale;
	//Quaternion rotation;
>>>>>>> cbb1e29bc125ff8c4a9d230abb164da70f956360

	void translate(XMMATRIX position);
	void rotate(float yaw, float pitch, float roll);

};





