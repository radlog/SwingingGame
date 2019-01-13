#pragma once
// get the sign of a number
inline int sign(const float number)
{
	return number < 0 ? -1 : (number > 0 ? 1 : 0);
}

// calculates the distance between two vectors
inline float dist(const XMVECTOR v1, const XMVECTOR v2)
{
	return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
}

// calculates the dot product of two vectors
inline float dot(const XMVECTOR v1, const XMVECTOR v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
// calculates the cross product of two vectors
inline XMVECTOR cross(const XMVECTOR v1, const XMVECTOR v2)
{
	return XMVectorSet(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x, 0);
}

// calculates the normal of 3 vectors representing a triangle of a plane
inline XMVECTOR normal(const XMVECTOR v1, const XMVECTOR v2, const XMVECTOR v3)
{
	const auto n1 = v1 - v2;
	const auto n2 = v1 - v3;
	const auto c = cross(n1, n2);
	const auto length = sqrt(pow(c.x, 2) + pow(c.y, 2) + pow(c.z, 2));
	return XMVectorSet(c.x / length, c.y / length, c.z / length, 0);
}

// plane equation
inline Plane get_plane(const XMVECTOR v1, const XMVECTOR v2, const XMVECTOR v3)
{
	const auto norm =  normal(v1, v2, v3);
	const auto d_offset =  -dot(v1, norm);

	return Plane{ norm, d_offset };
}
// determines whether the given point is on the given plane
inline float is_point_on_plane(const Plane *plane, const XMVECTOR *point)
{
	const auto n = plane->normal;
	return (n.x * point->x) + (n.y * point->y) + (n.z * point->z) + plane->offset;
}

// gives the point of intersection between the ray and the plane
inline XMVECTOR ray_to_plane_intersection_point(const Plane *plane, const XMVECTOR *ray, const XMVECTOR *start_point)
{
	// fraction between 0 and 1 that describes the intersection with the plane
	const auto fraction = (-plane->offset - dot(plane->normal, *start_point)) / dot(plane->normal, *ray);
	return *start_point + (*ray * fraction);
}

// uses the plane and two vectors to see if their ray intersects the plane
inline bool plane_intersection(const Plane *plane, const XMVECTOR *start_point, const XMVECTOR *end_point)
{
	const auto s1 = is_point_on_plane(plane, start_point);
	const auto s2 = is_point_on_plane(plane, end_point);

	return sign(s1) != sign(s2);	
}

// check if a point lies in a triangle
inline bool in_triangle(XMVECTOR *triangle_vector_a, XMVECTOR *triangle_vector_b, XMVECTOR *triangle_vector_c, const XMVECTOR *point)
{
	const auto v1 = *triangle_vector_b - *triangle_vector_a;
	const auto v1_p = *point - *triangle_vector_a;
	const auto v2 = *triangle_vector_c - *triangle_vector_b;
	const auto v2_p = *point - *triangle_vector_b;
	const auto v3 = *triangle_vector_a - *triangle_vector_c;
	const auto v3_p = *point - *triangle_vector_c;

	const auto n1 = dot(v1, v1_p);
	const auto n2 = dot(v2, v2_p);
	const auto n3 = dot(v3, v3_p);


	auto result = sign(n1) == sign(n2) && sign(n1) == sign(n3) && sign(n2) == sign(n3);// || n1 >= 0 && n2 >= 0 && n3 >= 0;

	return result;
}

