#pragma once
#include <xnamath.h>

class Transform
{
public:
	Transform();
	Transform(XMVECTOR scale, XMVECTOR rotation, XMVECTOR position);
	~Transform();

	XMMATRIX calculate_world_transform();

	void translate(XMVECTOR position);
	void rotate(float pitch, float yaw, float roll);
	void scale(float x, float y, float z);

	void rotate_fixed(float pitch, float yaw, float roll);

	void translate(XMVECTOR direction, float speed);
	void forward(float speed);
	void horizontal_forward(float speed);
	void right(float speed);
	void up(float speed);

	void look_at(XMVECTOR target);
	XMMATRIX get_world();
	XMVECTOR get_local_position() const;


	static const XMVECTOR world_up;
	static const XMVECTOR world_down;
	static const XMVECTOR world_left;
	static const XMVECTOR world_right;
	static const XMVECTOR world_forward;
	static const XMVECTOR world_backward;

	void apply_force(XMVECTOR force);

protected:


private:
	float y_rotation_sum_ = 0.0f;
	float y_rotation_max_ = 89.0f;

	XMVECTOR local_up_ = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMVECTOR local_down_ = -local_up_;
	XMVECTOR local_right_ = XMVectorSet(1.0, 0.0, 0.0, 0.0);
	XMVECTOR local_left_ = -local_right_;
	XMVECTOR local_forward_ = XMVectorSet(0.0, 0.0, 1.0, 0.0);
	XMVECTOR local_backward_ = -local_forward_;

	XMVECTOR local_scale_;
	XMVECTOR local_rotation_;
	XMVECTOR local_position_;
	XMMATRIX world_;


};

