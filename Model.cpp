#include "pch.h"
#include "Model.h"
#include "MeshCollider.h"

Model::Model(const LPCSTR filename, const CB_STATE state, const COLLIDER_TYPE col_type) : BaseModel(filename, state)
{
	switch (col_type)
	{
		case SPHERE: initialize_sphere_collider();  break;
		case MESH: initialize_mesh_collider(); break;
		default: initialize_sphere_collider();
	}
}

Model::Model(const CB_STATE state, const COLLIDER_TYPE col_type) : BaseModel(state)
{
	switch(col_type)
	{
		case SPHERE: initialize_sphere_collider();  break;
		case MESH: initialize_mesh_collider(); break;
		default: initialize_sphere_collider();
	}
}

Model::~Model()
{
}

void Model::draw(const XMMATRIX view_projection, const bool use_simple_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	BaseModel::draw(view_projection, use_simple_cb, mode);
}

void Model::add_sphere_collider(SphereCollider* col)
{
	collider_ = *col;
}

void Model::add_mesh_collider(MeshCollider* col)
{	
	collider_ = *col;
}

void Model::initialize_sphere_collider()
{
	calculate_origin();
	collider_ = static_cast<Collider>(SphereCollider());
}

void Model::initialize_mesh_collider()
{
	calculate_origin();
	collider_ = static_cast<Collider>(MeshCollider());
}

