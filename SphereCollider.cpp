#include "SphereCollider.h"


SphereCollider::SphereCollider(): origin_(XMVectorSet(0,0,0,0)), radius_(0)
{
}


SphereCollider::~SphereCollider()
{
}

bool SphereCollider::sphere_to_sphere_collision(const SphereCollider col) const
{
	return dist(col.origin_, origin_) >= col.radius_ + radius_;		
}

XMVECTOR SphereCollider::get_origin()
{
	return origin_;
}
