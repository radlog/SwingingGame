#include "pch.h"
#include "Floor.h"
#include "MathHelper.h"

const float default_height = -10.0f;

Floor::~Floor()
{
}

// initialize floor with tiles and plane scale
Floor::Floor(const int tiles, const float scale)
{
	this->tiles_ = tiles; // set tiles
	this->scale_ = scale; // set plane scale
	transform_ = new Transform(XMVectorSet(scale_, 1.0f, scale_, 0.0f), XMQuaternionIdentity(), XMVectorSet((-tiles_ * scale_) / 2, default_height, (-tiles_ * scale_) / 2, 0.0f)); // calculate transform depending on scale and tiles
	Geometry::plane_ittn(&plane_vertices_, &plane_indices_, tiles); // create geometry of plane
	load_geo_model(plane_vertices_, (tiles + 1)*(tiles + 1), sizeof(POS_TEX_NORM_COL_VERTEX), plane_indices_, tiles * tiles * 6); // load created plane geometry with indices
	load_texture(); // load default texture

	Floor::initialize_sphere_collider(); // initialize sphere collider
	Floor::initialize_mesh_collider(); // initialize mesh collider
	
}

// get transform
Transform* Floor::get_transform() const
{
	return transform_;
}
// get number of tiles
int Floor::get_tiles() const
{
	return tiles_;
}

// get scale of plane
float Floor::get_scale() const
{
	return scale_;
}

// initialize mesh collider
void Floor::initialize_mesh_collider()
{
#if DEBUG
	const auto local_position = transform_->get_local_position(); // get position of transform 
#endif
	const auto v1 = XMLoadFloat3(&plane_vertices_[0].pos); // set the position of first vertex of the plane
	const auto v2 = XMLoadFloat3(&plane_vertices_[tiles_ ].pos); // set the position of second vertex of the plane
	const auto v3 = XMLoadFloat3(&plane_vertices_[(tiles_ + 1) * (tiles_ + 1) -1].pos); // set the position of third vertex of the plane
	mesh_collider_ = new MeshCollider(origin_,vector<XMVECTOR> {v1,v2,v3}, radius_); // initialize mesh collider with origin, the three vertices and radius
}

// initialize sphere collider
void Floor::initialize_sphere_collider()
{	
	radius_ = radius_ * float(tiles_); // recalculate radius with number of tiles
	sphere_collider_ = new SphereCollider(origin_, radius_); // initialize sphere collider with radius and origin
}




