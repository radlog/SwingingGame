#include "SphereCollider.h"


SphereCollider::SphereCollider() : radius_(0)
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
	const auto distance = dist(col.world_, world_);
	const auto limit = col.radius_ + radius_;
	auto test = distance <= limit;
	return distance <= limit;
}

bool SphereCollider::sphere_to_mesh_collision(MeshCollider col) const
{
	for (auto& i : *col.get_triangles())
	{
		// transform vertices to the correct position in world space
		XMVECTOR v1 = i.v1 + col.get_world_position();
		XMVECTOR v2 = i.v2 + col.get_world_position();
		XMVECTOR v3 = i.v3 + col.get_world_position();

		auto plane = get_plane(v1, v2, v3); // get plane from vertices
		auto ray = XMVector4Normalize(world_ - plane.normal * col.get_world_position());
		auto start_point = world_;
		auto end_point = world_  + ray * radius_ + ray * col.get_radius();

		if (plane_intersection(&plane, &start_point, &end_point))
		{
			auto point = ray_to_plane_intersection_point(&plane, &ray, &start_point);
			if (in_triangle(&v1, &v2, &v3, &point))
				return true;
		}
	}
	return false;
}

bool SphereCollider::check_collision(Collider* col)
{
	if (typeid(*col).name() == typeid(SphereCollider).name())
	{
		const auto s = dynamic_cast<SphereCollider*>(col);
		return sphere_to_sphere_collision(*s);
	}

	const auto m = dynamic_cast<MeshCollider*>(col);
	return sphere_to_mesh_collision(*m);

}

float SphereCollider::get_radius() const
{
	return radius_;
}
