#include "SphereCollider.h"


SphereCollider::SphereCollider(): radius_(0)
{
}

SphereCollider::SphereCollider(XMVECTOR origin, float radius)
{
	origin_ = origin;
	radius_ = radius;
}


SphereCollider::~SphereCollider()
{
}

bool SphereCollider::sphere_to_sphere_collision(const SphereCollider col) const
{
	// return
	const auto distance = dist(col.world_, world_);
	const auto limit = col.radius_ + radius_;
	const auto bla = distance <= limit;
	return bla;
}

bool SphereCollider::sphere_to_mesh_collision(MeshCollider col) const
{
	return false;
}

bool SphereCollider::check_collision(Collider* col)
{
	if(typeid(*col).name() == typeid(SphereCollider).name())
	{
		const auto s = dynamic_cast<SphereCollider*>(col);
		return sphere_to_sphere_collision(*s);
	}else
	{
		const auto m = dynamic_cast<MeshCollider*>(col);
		return sphere_to_mesh_collision(*m);
	}

	return false;
}
