#include "pch.h"
#include "Floor.h"



//Floor::Floor()
//{
//}


Floor::~Floor()
{
}

/**
 * \brief 
 * \param texture 
 * \param tiles 
 * \param scale 
 */
Floor::Floor(const int tiles, const float scale)
{
	this->tiles_ = tiles;
	this->scale_ = scale;
	transform_ = new Transform(XMVectorSet(scale_, 1.0f, scale_, 0.0f), XMQuaternionIdentity(), XMVectorSet((-tiles_ * scale_) / 2, -10.0f, (-tiles_ * scale_) / 2, 0.0f));
	Geometry::plane_ittn(&plane_vertices_, &plane_indices_, tiles);
	load_geo_model(plane_vertices_, (tiles + 1)*(tiles + 1), sizeof(POS_TEX_NORM_COL_VERTEX), plane_indices_, tiles * tiles * 6);
	load_texture();

	Floor::initialize_mesh_collider();
	
}

Transform* Floor::get_transform() const
{
	return transform_;
}



void Floor::initialize_mesh_collider()
{
	const auto local_position = transform_->get_local_position();

	const auto v1 = XMLoadFloat3(&plane_vertices_[0].pos);// +local_position;
	const auto v2 = XMLoadFloat3(&plane_vertices_[tiles_ ].pos);// +local_position;
	const auto v3 = XMLoadFloat3(&plane_vertices_[(tiles_ + 1) * (tiles_ + 1) -1].pos);// +local_position;
	collider_ = new MeshCollider(origin_,vector<XMVECTOR> {v1,v2,v3});
}




