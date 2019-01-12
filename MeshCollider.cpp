#include "pch.h"
#include "MeshCollider.h"


MeshCollider::MeshCollider()
{
}

MeshCollider::MeshCollider(vector<XMVECTOR> *vertex_positions)
{
	//for(auto i = 0; i < vertex_positions->size(); i++)
	//{
	//	//triangles_.push_back(new Triangle{ vertex_positions[i], vertex_positions[i + 1] , vertex_positions[i + 2] });
	//}
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
