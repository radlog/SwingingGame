#include "Transform.h"



Transform::Transform()
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
	// return XMMatrixTransformation(scale, rotation, position);
	return NULL;
}
