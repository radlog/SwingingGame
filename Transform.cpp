#include "Transform.h"



Transform::Transform()
{
<<<<<<< HEAD
	//scale = ;
	//local_rotation = XMMatrixIdentity();
	//local_translation = XMMatrixIdentity();
=======
	//scale = XMMatrixIdentity();
	//rotation = XMMatrixIdentity();
	//translation = XMMatrixIdentity();
>>>>>>> refs/remotes/origin/master
	//world = get_world_transform();
	
}


Transform::~Transform()
{
}

XMMATRIX Transform::get_world_transform()
<<<<<<< HEAD
{	
	//return XMMATRIX( scale * rotation * position);
=======
{
	//return scale * rotation * translation;
>>>>>>> refs/remotes/origin/master
	return NULL;
}
