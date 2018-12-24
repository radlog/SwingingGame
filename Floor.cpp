#include "pch.h"
#include "Floor.h"



Floor::Floor()
{
}


Floor::~Floor()
{
}

Floor::Floor(LPCSTR texture, int tiles, float scale) : GameObject(texture)
{
	this->tiles = tiles;
	this->scale = scale;

	model = Model(dx_handle->device, dx_handle->immediateContext);
	Geometry::create_indexed_tiled_textured_normal_plane(&plane_vertices, &plane_indices, tiles, scale);
	model.LoadGeoModel(plane_vertices, (tiles + 1)*(tiles + 1), sizeof(POS_TEX_NORM_COL_VERTEX), plane_indices, tiles * tiles * 6);

	model.LoadTexture();

	transform = Transform(XMVectorSet(scale, 1.0f, scale, 0.0f), XMQuaternionIdentity(), XMVectorSet((-tiles * scale) / 2, -10.0f, (-tiles * scale) / 2, 0.0f));

	get_collider();
	//plane_collider = get_plane(XMLoadFloat3(&plane_vertices[0].Pos) + transform.local_position, XMLoadFloat3(&plane_vertices[tiles + 1].Pos) + transform.local_position, XMLoadFloat3(&plane_vertices[(tiles + 1) * (tiles + 1)].Pos) + transform.local_position);
	
}

Plane Floor::get_collider()
{
	return plane_collider = get_plane(XMLoadFloat3(&plane_vertices[0].Pos) + transform.local_position, XMLoadFloat3(&plane_vertices[tiles + 1].Pos) + transform.local_position, XMLoadFloat3(&plane_vertices[(tiles + 1) * (tiles + 1)].Pos) + transform.local_position);
}