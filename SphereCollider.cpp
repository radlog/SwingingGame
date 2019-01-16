#include "SphereCollider.h"
#include "MathHelper.h"

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
	const auto distance = dist(col.world_, world_); // get distance to other sphere collider
	const auto limit = col.radius_ + radius_; // set limit adding radius of both together	
	return distance <= limit; // return true if the distance is less than the limit
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
		auto ray = XMVector4Normalize(world_ - plane.normal * col.get_world_position()); // create ray pointing towards the mesh colliding with		
		auto end_point = world_  + ray * radius_ + ray * col.get_radius(); // cast ray multiplied with collider radius to prevent multiple collisions with planes of the mesh

		if (plane_intersection(&plane, &world_, &end_point)) // check if ray intersected with plane
		{
			auto point = ray_to_plane_intersection_point(&plane, &ray, &world_); // get intersection point
			if (in_triangle(&v1, &v2, &v3, &point)) // check if intersection point is in triangle
				return true;
		}
	}
	return false;
}

bool SphereCollider::check_collision(Collider* col)
{
	// call the corresponding function depending on which type the collider is
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
