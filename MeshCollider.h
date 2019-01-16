#pragma once
#include "Collider.h"
#include "Geometry.h"

class MeshCollider :
	public Collider
{
public:
	MeshCollider();
	explicit MeshCollider(XMVECTOR origin, vector<XMVECTOR> vertex_positions, float radius); // initialize collider with origin, vertex positions, radius
	~MeshCollider();

	vector<Triangle>* get_triangles(); // return triangle data of collider
	float get_radius() const; // return bounding sphere radius of collider

private:
	vector<Triangle> *triangles_; // triangle data for the collider
	float radius_; // radius to double check collision after sphere collision in scene
};

