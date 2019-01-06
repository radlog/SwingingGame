#include "Transform.h"


const XMVECTOR Transform::world_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
const XMVECTOR Transform::world_down = -world_up;
const XMVECTOR Transform::world_right = XMVectorSet(1.0, 0.0, 0.0, 0.0);
const XMVECTOR Transform::world_left = -world_right;
const XMVECTOR Transform::world_forward = XMVectorSet(0.0, 0.0, 1.0, 0.0);
const XMVECTOR Transform::world_backward = -world_forward;


Transform::Transform() : local_scale_(XMVectorSet(1.0, 1.0, 1.0, 0.0)), local_rotation_(XMQuaternionIdentity()), local_position_(XMVectorSet(0.0, 0.0, 0.0, 0.0))
{	
	world_ = XMMatrixScalingFromVector(local_scale_) * XMMatrixRotationQuaternion(local_rotation_) * XMMatrixTranslationFromVector(local_position_);
}

Transform::Transform(const XMVECTOR scale, const XMVECTOR rotation, const XMVECTOR position)
{
	local_scale_ = scale;
	local_rotation_ = rotation;
	local_position_ = position;
	world_ = XMMatrixScalingFromVector(local_scale_) * XMMatrixRotationQuaternion(local_rotation_) * XMMatrixTranslationFromVector(local_position_);
}


Transform::~Transform()
{
}

XMMATRIX Transform::calculate_world_transform()
{	
	return world_ = XMMatrixScalingFromVector(local_scale_) * XMMatrixRotationQuaternion(local_rotation_) * XMMatrixTranslationFromVector(local_position_);	
}

void Transform::translate(const XMVECTOR position)
{
	local_position_ = position;
	calculate_world_transform();
}


void Transform::rotate_fixed(float pitch, const float yaw, const float roll)
{
	// TODO:: study this more
	const auto dot = XMVector3Dot(local_forward_, world_up).x;
	const auto theta = XMScalarACos(dot) - XM_PI / 2;
	const auto min = XMConvertToRadians(-89) - theta;
	const auto max = XMConvertToRadians(89) - theta;
	pitch = XMMin(max, XMMax(pitch, min));

	rotate(pitch, yaw, roll);
}

void Transform::rotate(const float pitch, const float yaw, const float roll)
{
	local_rotation_ = XMQuaternionMultiply(XMQuaternionRotationRollPitchYaw(pitch, 0, roll), XMQuaternionMultiply(local_rotation_, XMQuaternionRotationAxis(world_up, yaw)));
	const auto rot_matrix = XMMatrixRotationQuaternion(local_rotation_);
	local_forward_ = XMVector4Transform(world_forward, rot_matrix);
	local_backward_ = -local_forward_;
	local_right_ = XMVector4Transform(world_right, rot_matrix);
	local_left_ = -local_right_;
	local_up_ = XMVector4Transform(world_up, rot_matrix);
	local_down_ = -local_up_;
	calculate_world_transform();
}

void Transform::scale(const float x, const float y, const float z)
{
	local_scale_ = XMVectorSet(x, y, z, 0.0f);
	calculate_world_transform();
}

void Transform::translate(const XMVECTOR direction, const float speed)
{	
	local_position_ += XMVector4Normalize(direction) * speed;
	calculate_world_transform();
}

void Transform::forward(const float speed)
{
	local_position_ += XMVector4Normalize(local_forward_) * speed;
	calculate_world_transform();
}

void Transform::horizontal_forward(const float speed)
{
	const auto forward_new = XMVector3Normalize(XMVECTOR(XMVectorSet(local_forward_.x, 0, local_forward_.z, 0)));
	local_position_ += XMVector4Normalize(forward_new) * speed;
	calculate_world_transform();
}

void Transform::right(const float speed)
{
	local_position_ += XMVector4Normalize(local_right_) * speed;
	calculate_world_transform();
}

void Transform::up(const float speed)
{
	const auto up_new = XMVector3Normalize(XMVECTOR(XMVectorSet(0, local_up_.y, 0, 0)));
	local_position_ += XMVector4Normalize(up_new) * speed;
	calculate_world_transform();
}

void Transform::look_at(XMVECTOR target)
{

}

XMMATRIX Transform::get_world()
{
	return world_;
}

XMVECTOR Transform::get_local_position() const
{
	return local_position_;
}

void Transform::apply_force(XMVECTOR force)
{
	local_position_ += XMVector4Normalize(force);
	calculate_world_transform();
}


