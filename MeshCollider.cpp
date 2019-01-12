#include "pch.h"
#include "MeshCollider.h"


MeshCollider::MeshCollider()
{
}

MeshCollider::MeshCollider(vector<XMVECTOR> vertex_positions)
{
	triangles_ = new vector<Triangle>();
	for (auto i = 0; i < vertex_positions.size() - 3; i+= 3)
	{
		triangles_->push_back(Triangle{ vertex_positions[i], vertex_positions[i + 1] , vertex_positions[i + 2] });
	}
}


MeshCollider::~MeshCollider()
{
}

bool MeshCollider::check_collision(Collider* col)
{
	if (typeid(*col).name() == typeid(SphereCollider).name())
		return sphere_to_mesh_collision(dynamic_cast<SphereCollider*>(col));
	return mesh_to_mesh_collision(dynamic_cast<MeshCollider*>(col));
	
}

bool MeshCollider::mesh_to_mesh_collision(MeshCollider* col)
{
	return false;
}

bool MeshCollider::sphere_to_mesh_collision(SphereCollider* col)
{
	return false;
}
