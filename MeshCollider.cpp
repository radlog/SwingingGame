#include "pch.h"
#include "MeshCollider.h"


MeshCollider::MeshCollider()
{
}

MeshCollider::MeshCollider(XMVECTOR normal, float offset)
{
}


MeshCollider::~MeshCollider()
{
}

bool MeshCollider::check_collision(Collider* col)
{
	return false;
}

bool MeshCollider::mesh_to_mesh_collision(MeshCollider* col)
{
	return false;
}
