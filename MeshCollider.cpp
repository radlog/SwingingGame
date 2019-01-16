#include "pch.h"
#include "MeshCollider.h"


MeshCollider::MeshCollider()
{
}

MeshCollider::MeshCollider(XMVECTOR origin, vector<XMVECTOR> vertex_positions, float radius) : triangles_(nullptr)
{
	origin_ = origin;
	world_ = origin;
	radius_ = radius;
	triangles_ = new vector<Triangle>();
	// fill triangle data with vertex positions
	for (auto i = 0; i < vertex_positions.size() - 2; i+= 3)
	{
		triangles_->push_back(Triangle{ vertex_positions[i], vertex_positions[i + 1] , vertex_positions[i + 2] });
	}
}

MeshCollider::~MeshCollider()
{
}

vector<Triangle>* MeshCollider::get_triangles()
{
	return triangles_;
}

float MeshCollider::get_radius() const
{
	return radius_;
}
