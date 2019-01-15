#include "pch.h"
#include "Model.h"
#include "MeshCollider.h"

Model::Model(const LPCSTR filename, const CB_STATE state) : BaseModel(filename, state)
{
	origin_ = XMVectorZero();
	calculate_origin();
	Model::initialize_sphere_collider();
	Model::initialize_mesh_collider();
}

Model::Model(const CB_STATE state) : BaseModel(state)
{
	origin_ = XMVectorZero();
	calculate_origin();
	Model::initialize_sphere_collider();
	Model::initialize_mesh_collider();
}

Model::~Model()
{
}

void Model::draw(const XMMATRIX view_projection, const bool use_simple_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	BaseModel::draw(view_projection, use_simple_cb, mode);
}

SphereCollider* Model::get_bounding_sphere() const
{
	return sphere_collider_;
}

MeshCollider* Model::get_mesh_collider() const
{
	return mesh_collider_;
}


void Model::initialize_sphere_collider()
{
	radius_ = 0;
	if (obj_file_model_ != nullptr) radius_ = dist(min_outer_vector_, max_outer_vector_) ;
	else radius_ = 0.2f;
	sphere_collider_ = new SphereCollider(origin_, radius_);
}

void Model::initialize_mesh_collider()
{
	if (obj_file_model_ == nullptr) return;

	mesh_collider_ = new MeshCollider(origin_, obj_file_model_->get_vertex_positions(),radius_);
}

