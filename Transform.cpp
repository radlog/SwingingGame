#include "Transform.h"

// static world transforms 
const XMVECTOR Transform::world_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
const XMVECTOR Transform::world_down = -world_up;
const XMVECTOR Transform::world_right = XMVectorSet(1.0, 0.0, 0.0, 0.0);
const XMVECTOR Transform::world_left = -world_right;
const XMVECTOR Transform::world_forward = XMVectorSet(0.0, 0.0, 1.0, 0.0);
const XMVECTOR Transform::world_backward = -world_forward;


Transform::Transform() : world_scale_(XMVectorSplatOne()), local_scale_(XMVectorSet(1.0, 1.0, 1.0, 0.0)), local_rotation_(XMQuaternionIdentity()), local_position_(XMVectorSet(0.0, 0.0, 0.0, 0.0))
{
	calculate_world_transform(); // calculate world matrix
}

Transform::Transform(const XMVECTOR scale, const XMVECTOR rotation, const XMVECTOR position)
{
	local_scale_ = scale;
	local_rotation_ = rotation;
	local_position_ = position;
	world_scale_ = scale;
	world_rotation_ = rotation;
	world_position_ = position;
	local_world_ = XMMatrixScalingFromVector(local_scale_) * XMMatrixRotationQuaternion(local_rotation_) *
		XMMatrixTranslationFromVector(local_position_);
	world_ = XMMatrixScalingFromVector(local_scale_) * XMMatrixRotationQuaternion(local_rotation_) *
		XMMatrixTranslationFromVector(local_position_);
}


Transform::~Transform()
{
}

// calculate world matrix
XMMATRIX Transform::calculate_world_transform()
{
	// multiply transform components to get correct world matrix
	return world_ = XMMatrixScalingFromVector(local_scale_) * XMMatrixRotationQuaternion(local_rotation_) * XMMatrixTranslationFromVector(local_position_);
}

// move to a position
void Transform::translate(const XMVECTOR position)
{
	local_position_ = position; // set position
	calculate_world_transform(); // recalculate world matrix
}

// rotate with a locked angle for the x axis around pitch,yaw,roll
void Transform::rotate_fixed(float pitch, const float yaw, const float roll)
{
	const auto theta = XMScalarACos(local_forward_.y) - XM_PI / 2; // get angle of current local_forward along the x axis and subtract 90 degrees to have the limit value
	const auto min = XMConvertToRadians(-89) - theta; // get minimum rotation angle using the limit value 
	const auto max = XMConvertToRadians(89) - theta; // get maximum rotation angle using the limit value 
	pitch = XMMin(max, XMMax(pitch, min)); // set rotation on x axis to be locked at minimum and maximum rotation angle

	rotate(pitch, yaw, roll); // call default rotate function
}

// rotate around pitch,yaw,roll
void Transform::rotate(const float pitch, const float yaw, const float roll)
{
	// get new rotation by multiplying the new rotation with the old one ignoring the y axis
	local_rotation_ = XMQuaternionMultiply(XMQuaternionRotationRollPitchYaw(pitch, 0, roll), XMQuaternionMultiply(local_rotation_, XMQuaternionRotationAxis(world_up, yaw))); 
	const auto rot_matrix = XMMatrixRotationQuaternion(local_rotation_); // create rotation matrix
	local_forward_ = XMVector4Transform(world_forward, rot_matrix); // recalculate forward vector
	local_backward_ = -local_forward_; // set backward vector to the opposite of the forward vector
	local_right_ = XMVector4Transform(world_right, rot_matrix); // recalculate right vector
	local_left_ = -local_right_; // set left vector to the opposite of the right vector
	local_up_ = XMVector4Transform(world_up, rot_matrix); // recalculate up vector
	local_down_ = -local_up_; // set down vector to the opposite of the up vector
	calculate_world_transform(); // recalculate world matrix
}

// set local scale
void Transform::scale(const float x, const float y, const float z)
{
	local_scale_ = XMVectorSet(x, y, z, 0.0f); 
	calculate_world_transform();
}

// move in a direction at a given speed
void Transform::translate(const XMVECTOR direction, const float speed)
{
	local_position_ += XMVector4Normalize(direction) * speed;
	calculate_world_transform();
}

// get world matrix
XMMATRIX Transform::get_world()
{
	return world_;
}

// get local position
XMVECTOR Transform::get_local_position() const
{
	return local_position_;
}

// set world matrix
void Transform::set_world(const XMMATRIX local_world)
{
	world_ = local_world;
	calculate_world_transform();
}

// get local world matrix
XMMATRIX Transform::get_local_world() const
{
	return XMMatrixScalingFromVector(local_scale_) * XMMatrixRotationQuaternion(local_rotation_) * XMMatrixTranslationFromVector(local_position_);
}

// set world position
void Transform::set_world_position(XMVECTOR position)
{
	world_position_ = position;
	calculate_world_transform();
}

// get local scale
XMVECTOR Transform::get_local_scale() const
{
	return local_scale_;
}

// get local scale
XMVECTOR Transform::get_world_scale() const
{
	return world_scale_;
}

// set world scale
void Transform::set_world_scale(const XMVECTOR scale)
{
	world_scale_ = scale;
}

// get local forward vector
XMVECTOR Transform::get_local_forward() const
{
	return XMVector3Normalize(local_forward_);
}

// get local forward vector ignoring the y axis
XMVECTOR Transform::get_local_forward_horizontal() const
{
	return XMVector3Normalize(XMVectorSet(local_forward_.x, 0, local_forward_.z, 0));
}

// get local up vector
XMVECTOR Transform::get_local_up() const
{
	return XMVector3Normalize(local_up_);
}

// get local right vector
XMVECTOR Transform::get_local_right() const
{
	return XMVector3Normalize(local_right_);
}


