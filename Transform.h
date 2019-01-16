#pragma once
#include <xnamath.h>

class GameObject;

class Transform
{


public:
	Transform();
	Transform(XMVECTOR scale, XMVECTOR rotation, XMVECTOR position);
	~Transform();

	XMMATRIX calculate_world_transform(); // get world transform by multiplying scale, rotation, position

	void translate(XMVECTOR position); // move to a given position
	void rotate(float pitch, float yaw, float roll); // rotate all 3 axis
	void scale(float x, float y, float z); // scale to x,y,z

	void rotate_fixed(float pitch, float yaw, float roll); // rotate all 3 axis but ignore rotation on x axis if it exceed the rotation limit

	void translate(XMVECTOR direction, float speed); // moves in a direction at given value(speed)

	XMMATRIX get_world(); // return world matrix
	XMVECTOR get_local_position() const; // get local position
	void set_world(XMMATRIX local_world); // set world matrix
	XMMATRIX get_local_world() const; // get local 
	void set_world_position(XMVECTOR position);


	// static world directions
	static const XMVECTOR world_up;
	static const XMVECTOR world_down;
	static const XMVECTOR world_left;
	static const XMVECTOR world_right;
	static const XMVECTOR world_forward;
	static const XMVECTOR world_backward;


	XMVECTOR get_local_scale() const; // return local scale
	XMVECTOR get_world_scale() const; // return world scale
	void set_world_scale(XMVECTOR scale); // set world scale

	// get local directions
	XMVECTOR get_local_forward() const;
	XMVECTOR get_local_forward_horizontal() const;
	XMVECTOR get_local_up() const;
	XMVECTOR get_local_right() const;


private:
	float x_rotation_sum_ = 0.0f; // minimum angle for the rotation along the x axis
	float x_rotation_max_ = 89.0f; // maximum angle for the rotation along the x axis

	// local direction vectors
	XMVECTOR local_up_ = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMVECTOR local_down_ = -local_up_;
	XMVECTOR local_right_ = XMVectorSet(1.0, 0.0, 0.0, 0.0);
	XMVECTOR local_left_ = -local_right_;
	XMVECTOR local_forward_ = XMVectorSet(0.0, 0.0, 1.0, 0.0);
	XMVECTOR local_backward_ = -local_forward_;

	// world transform vectors
	XMVECTOR world_scale_;
	XMVECTOR world_rotation_;
	XMVECTOR world_position_;
	XMMATRIX world_;

	// local transform
	XMVECTOR local_scale_;
	XMVECTOR local_rotation_;
	XMVECTOR local_position_;
	XMMATRIX local_world_;
};

