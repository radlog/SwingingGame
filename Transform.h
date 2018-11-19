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
>>>>>>> refs/remotes/origin/master

	void translate(XMMATRIX position);
	void rotate(float yaw, float pitch, float roll);

};





