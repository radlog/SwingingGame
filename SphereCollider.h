#pragma once
#include "Collider.h"
class SphereCollider :
	public Collider
{
public:
	SphereCollider();
	SphereCollider(XMVECTOR origin, float radius);
	~SphereCollider();

	bool sphere_to_sphere_collision(SphereCollider col) const;

	bool sphere_to_mesh_collision(MeshCollider col) const;
	bool check_collision(Collider *col) override;
	float get_radius()const ;
private:
	float radius_;
};

