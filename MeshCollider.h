#pragma once
#include "Collider.h"
class MeshCollider :
	public Collider
{
public:
	MeshCollider();
	MeshCollider(XMVECTOR normal, float offset);
	~MeshCollider();
	bool check_collision(Collider* col) override;

	bool mesh_to_mesh_collision(MeshCollider *col);

private:
	Plane plane_ = { XMVectorSet(0,0,0,0),0 };
};

