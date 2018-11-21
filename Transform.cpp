#include "Transform.h"
#include <valarray>
#include <string>
#include <cmath>

const XMVECTOR Transform::world_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
const XMVECTOR Transform::world_down = -world_up;
const XMVECTOR Transform::world_right = XMVectorSet(1.0, 0.0, 0.0, 0.0);
const XMVECTOR Transform::world_left = -world_right;
const XMVECTOR Transform::world_forward = XMVectorSet(0.0, 0.0, 1.0, 0.0);
const XMVECTOR Transform::world_backward = -world_forward;


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

void Transform::translate(const XMVECTOR position)
{
	local_position = position;
}


void Transform::rotate(const float pitch, const float yaw, const float roll)
{
	local_rotation = XMQuaternionMultiply(XMQuaternionRotationRollPitchYaw(pitch, 0, roll), XMQuaternionMultiply(local_rotation, XMQuaternionRotationAxis(world_up, yaw)));
	
	const auto transformed = XMVector3Rotate(world_up, local_rotation);
	auto flattened = transformed - XMVector3Dot(transformed, local_right) * local_right;
	flattened = XMVector3Normalize(flattened);
	const auto dot = XMVector3Dot(world_up, flattened).x;
	const auto cross = XMVector3Cross(world_up, flattened);
	const auto cd = XMVector3Dot(cross, local_right).x;
	const auto sign = (0 < cd) - (cd < 0);
	const auto theta = std::acos(dot) * sign;
	const auto DEG2RAD = M_PI / 180.0;
	const auto d = theta + (89.0 * DEG2RAD);
	if(d < 0)
	{
		OutputDebugString((std::to_string(dot) + " " + std::to_string(theta) + "\n").c_str());
		OutputDebugString((std::to_string(d) + "\n").c_str());
		local_rotation = XMQuaternionMultiply(XMQuaternionRotationAxis(world_right, -d), local_rotation);
	}

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

void Transform::right(const float speed)
{
	local_position += local_right * speed;
}

void Transform::up(const float speed)
{
	local_position += local_up * speed;
}


