#include "pch.h"
#include "Floor.h"



Floor::Floor()
{
}


Floor::~Floor()
{
}

/**
 * \brief 
 * \param texture 
 * \param tiles 
 * \param scale 
 */
Floor::Floor(LPCSTR texture, int tiles, float scale) : GameObject(texture)
{
	this->tiles_ = tiles;
	this->scale_ = scale;

	model_ = new Model();
	Geometry::plane_ittn(&plane_vertices_, &plane_indices_, tiles, scale);
	model_->load_geo_model(plane_vertices_, (tiles + 1)*(tiles + 1), sizeof(POS_TEX_NORM_COL_VERTEX), plane_indices_, tiles * tiles * 6);
	model_->load_texture();

	transform = Transform(XMVectorSet(scale, 1.0f, scale, 0.0f), XMQuaternionIdentity(), XMVectorSet((-tiles * scale) / 2, -10.0f, (-tiles * scale) / 2, 0.0f));

	get_collider();
	//plane_collider = get_plane(XMLoadFloat3(&plane_vertices[0].Pos) + transform.local_position, XMLoadFloat3(&plane_vertices[tiles + 1].Pos) + transform.local_position, XMLoadFloat3(&plane_vertices[(tiles + 1) * (tiles + 1)].Pos) + transform.local_position);
	
}

//Plane Floor::get_collider()
//{
//	const auto local_position = transform.get_local_position();
//	const auto v1 = XMLoadFloat3(&plane_vertices_[0].pos) + local_position;
//	const auto v2 = XMLoadFloat3(&plane_vertices_[tiles_ + 1].pos) + local_position;
//	const auto v3 = XMLoadFloat3(&plane_vertices_[(tiles_ + 1) * (tiles_ + 1)].pos) + local_position;
//	return plane_collider_ = get_plane(v1, v2, v3);
//}



