#include "Transform.h"

Transform::Transform() : scale(XMVectorSet(1.0, 1.0, 1.0, 0.0)), rotation(XMQuaternionIdentity()), position(XMVectorSet(0.0, 0.0, 0.0, 0.0))
{
	//scale = XMMatrixIdentity();
	//rotation = XMMatrixIdentity();
	//translation = XMMatrixIdentity();
	//world = get_world_transform();

}


Transform::~Transform()
{
}

XMMATRIX Transform::get_world_transform()
{
	world = XMMatrixScalingFromVector(scale) * XMMatrixRotationQuaternion(rotation) * XMMatrixTranslationFromVector(position);
	return world;
}

void Transform::translate(XMVECTOR position)
{
	this->position = position;
}


void Transform::rotate(float yaw, float pitch, float roll)
{
	rotation = XMQuaternionMultiply(rotation, XMQuaternionRotationRollPitchYaw(yaw, pitch, roll));
}
