#pragma once
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
inline bool is_point_on_plane(const Plane plane, const XMVECTOR point)
{
	const auto n = plane.normal;
	return (n.x * point.x) + (n.y * point.y) + (n.z * point.z) + plane.offset == 0;
}