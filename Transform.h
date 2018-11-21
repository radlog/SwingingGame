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
	XMVECTOR local_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMVECTOR local_down = -local_up;
	XMVECTOR local_right = XMVectorSet(1.0, 0.0, 0.0, 0.0);
	XMVECTOR local_left = -local_right;
	XMVECTOR local_forward = XMVectorSet(0.0, 0.0, 1.0, 0.0);
	XMVECTOR local_backward = -local_forward;

	XMVECTOR local_scale;
	XMVECTOR local_rotation;
	XMVECTOR local_position;
	XMMATRIX world; 

	void translate(XMVECTOR position);
	void rotate(float yaw, float pitch, float roll);
	void scale(float x, float y, float z) const;

	void translate(XMVECTOR direction, float speed);
	void forward(float speed);
	void right(float speed);
	void up(float speed);


	static const XMVECTOR world_up;
	static const XMVECTOR world_down;
	static const XMVECTOR world_left;
	static const XMVECTOR world_right;
	static const XMVECTOR world_forward;
	static const XMVECTOR world_backward;

protected:


private:

};

