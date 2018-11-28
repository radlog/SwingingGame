#include "Transform.h"


const XMVECTOR Transform::world_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
const XMVECTOR Transform::world_down = -world_up;
const XMVECTOR Transform::world_right = XMVectorSet(1.0, 0.0, 0.0, 0.0);
const XMVECTOR Transform::world_left = -world_right;
const XMVECTOR Transform::world_forward = XMVectorSet(0.0, 0.0, 1.0, 0.0);
const XMVECTOR Transform::world_backward = -world_forward;


Transform::Transform() : local_scale(XMVectorSet(1.0, 1.0, 1.0, 0.0)), local_rotation(XMQuaternionIdentity()), local_position(XMVectorSet(0.0, 0.0, 0.0, 0.0))
{	
	world = XMMatrixScalingFromVector(local_scale) * XMMatrixRotationQuaternion(local_rotation) * XMMatrixTranslationFromVector(local_position);
}

Transform::Transform(XMVECTOR scale, XMVECTOR rotation, XMVECTOR position)
{
	local_scale = scale;
	local_rotation = rotation;
	local_position = position;
	world = XMMatrixScalingFromVector(local_scale) * XMMatrixRotationQuaternion(local_rotation) * XMMatrixTranslationFromVector(local_position);
}


Transform::~Transform()
{
}

XMMATRIX Transform::calculate_world_transform()
{	
	world = XMMatrixScalingFromVector(local_scale) * XMMatrixRotationQuaternion(local_rotation) * XMMatrixTranslationFromVector(local_position);
	return world;
}

void Transform::translate(const XMVECTOR position)
{
	local_position = position;
}


void Transform::rotate_fixed(float pitch, const float yaw, const float roll)
{
	// TODO:: study this more
	const auto dot = XMVector3Dot(local_forward, world_up).x;
	const auto theta = XMScalarACos(dot) - XM_PI / 2;
	const auto min = XMConvertToRadians(-89) - theta;
	const auto max = XMConvertToRadians(89) - theta;
	pitch = XMMin(max, XMMax(pitch, min));

	rotate(pitch, yaw, roll);
}

void Transform::rotate(const float pitch, const float yaw, const float roll)
{
	local_rotation = XMQuaternionMultiply(XMQuaternionRotationRollPitchYaw(pitch, 0, roll), XMQuaternionMultiply(local_rotation, XMQuaternionRotationAxis(world_up, yaw)));
	const auto rot_matrix = XMMatrixRotationQuaternion(local_rotation);
	local_forward = XMVector4Transform(world_forward, rot_matrix);
	local_backward = -local_forward;
	local_right = XMVector4Transform(world_right, rot_matrix);
	local_left = -local_right;
	local_up = XMVector4Transform(world_up, rot_matrix);
	local_down = -local_up;
}

void Transform::scale(float x, float y, float z) const
{
}

void Transform::translate(const XMVECTOR direction, const float speed)
{
	local_position += direction * speed;
}

void Transform::forward(const float speed)
{
	local_position += local_forward * speed;
}

void Transform::horizontal_forward(const float speed)
{
	const auto forward_new = XMVector3Normalize(XMVECTOR(XMVectorSet(local_forward.x, 0, local_forward.z, 0)));
	local_position += forward_new * speed;
}

void Transform::right(const float speed)
{
	local_position += local_right * speed;
}

void Transform::up(const float speed)
{
	local_position += local_up * speed;
}

void Transform::look_at(XMVECTOR target)
{

}


