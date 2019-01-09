#pragma once
#include "Collider.h"
class SphereCollider :
	public Collider
{
public:
	SphereCollider();
	~SphereCollider();

	bool sphere_to_sphere_collision(const SphereCollider col) const;
	XMVECTOR get_origin();


private:
	XMVECTOR origin_;
	float radius_;
};

