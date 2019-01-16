#pragma once
#include "Collider.h"
class SphereCollider :
	public Collider
{
public:
	SphereCollider();
	SphereCollider(XMVECTOR origin, float radius); // initialize sphere collider with origin, radius
	~SphereCollider();

	bool sphere_to_sphere_collision(SphereCollider col) const; // check collision to other sphere collider

	bool sphere_to_mesh_collision(MeshCollider col) const; // check collision to mesh collider
	bool check_collision(Collider *col) override; // check collision to any collider
	float get_radius()const; // return collider radius
private:
	float radius_; // bounding sphere radius
};

