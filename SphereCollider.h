#pragma once
#include "Collider.h"
class SphereCollider :
	public Collider
{
public:
	SphereCollider();
	SphereCollider(XMVECTOR origin, float radius);
	~SphereCollider();

	bool sphere_to_sphere_collision(const SphereCollider col) const;	

	bool check_collision(Collider *col) override;

private:
	float radius_;
};

