#include "Transform.h"



Transform::Transform()
{
	//scale = ;
	//local_rotation = XMMatrixIdentity();
	//local_translation = XMMatrixIdentity();
	//world = get_world_transform();
	
}


Transform::~Transform()
{
}

XMMATRIX Transform::get_world_transform()
{	
	//return XMMATRIX( scale * rotation * position);
	return NULL;
}
