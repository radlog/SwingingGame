#pragma once
#include "Collider.h"
#include "Geometry.h"

class MeshCollider :
	public Collider
{
public:
	MeshCollider();
	explicit MeshCollider(XMVECTOR origin, vector<XMVECTOR> vertex_positions, float radius);
	explicit MeshCollider(Plane* plane);
	~MeshCollider();
	bool check_collision(Collider* col) override;

	bool mesh_to_mesh_collision(MeshCollider *col);
	bool sphere_to_mesh_collision(SphereCollider *col);

	vector<Triangle>* get_triangles();
	float get_radius() const;

private:
	vector<Triangle> *triangles_;
	float radius_;
	//Plane* plane_;
};

