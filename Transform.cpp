#include "Transform.h"

const XMVECTOR Transform::world_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
const XMVECTOR Transform::world_down = XMVectorSet(0.0, -1.0, 0.0, 0.0);
const XMVECTOR Transform::world_left = XMVectorSet(-1.0, 0.0, 0.0, 0.0);
const XMVECTOR Transform::world_right = XMVectorSet(1.0, 1.0, 0.0, 0.0);
const XMVECTOR Transform::world_forward = XMVectorSet(0.0, 0.0, 1.0, 0.0);
const XMVECTOR Transform::world_backward = XMVectorSet(0.0, 0.0, -1.0, 0.0);


Transform::Transform() : local_scale(XMVectorSet(1.0, 1.0, 1.0, 0.0)), local_rotation(XMQuaternionIdentity()), local_position(XMVectorSet(0.0, 0.0, 0.0, 0.0))
{	
}


Transform::~Transform()
{
}

XMMATRIX Transform::get_world_transform()
{
	world = XMMatrixScalingFromVector(local_scale) * XMMatrixRotationQuaternion(local_rotation) * XMMatrixTranslationFromVector(local_position);
	return world;
}

void Transform::translate(XMVECTOR position)
{
	local_position = position;
}


void Transform::rotate(float yaw, float pitch, float roll)
{
	local_rotation = XMQuaternionMultiply(local_rotation, XMQuaternionRotationRollPitchYaw(yaw, pitch, roll));
	const auto rot_matrix = XMMatrixRotationQuaternion(local_rotation);
	local_forward = XMVector4Transform(XMVectorSet(0.0, 0.0, 1.0, 0.0), rot_matrix);
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


