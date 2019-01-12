#pragma once
#include "Collider.h"
#include "Geometry.h"

class MeshCollider :
	public Collider
{
public:
	MeshCollider();
	explicit MeshCollider(vector<XMVECTOR> *vertex_positions);
	~MeshCollider();
	bool check_collision(Collider* col) override;

	bool mesh_to_mesh_collision(MeshCollider *col);

private:
	vector<Triangle*> triangles_;
};

