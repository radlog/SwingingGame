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
>>>>>>> cbb1e29bc125ff8c4a9d230abb164da70f956360
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
>>>>>>> cbb1e29bc125ff8c4a9d230abb164da70f956360
	return NULL;
}
